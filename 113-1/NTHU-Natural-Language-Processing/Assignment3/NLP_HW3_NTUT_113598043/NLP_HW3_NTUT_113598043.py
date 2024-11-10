# %%
import transformers as T
from datasets import load_dataset
import torch
import os

from torch.utils.data import Dataset, DataLoader
from torch.optim import AdamW
from tqdm import tqdm
from torchmetrics import SpearmanCorrCoef, Accuracy, F1Score

device = "cuda" if torch.cuda.is_available() else "cpu"
os.makedirs('./saved_models', exist_ok=True)

# %%
# 有些中文的標點符號在tokenizer編碼以後會變成[UNK]，所以將其換成英文標點
token_replacement = [
    ["：" , ":"],
    ["，" , ","],
    ["“" , "\""],
    ["”" , "\""],
    ["？" , "?"],
    ["……" , "..."],
    ["！" , "!"]
]

# %%
tokenizer = T.BertTokenizer.from_pretrained("google-bert/bert-base-uncased", cache_dir="./cache/")

# %%
class SemevalDataset(Dataset):
    def __init__(self, split="train") -> None:
        super().__init__()
        assert split in ["train", "validation"]
        self.data = load_dataset(
            "sem_eval_2014_task_1", split=split, cache_dir="./cache/"
        ).to_list()

    def __getitem__(self, index):
        d = self.data[index]
        # 把中文標點替換掉
        for k in ["premise", "hypothesis"]:
            for tok in token_replacement:
                d[k] = d[k].replace(tok[0], tok[1])
        return d

    def __len__(self):
        return len(self.data)

data_sample = SemevalDataset(split="train").data[:3]
print(f"Dataset example: \n{data_sample[0]} \n{data_sample[1]} \n{data_sample[2]}")

# %%
# Define the hyperparameters
lr = 3e-5
epochs = 3
train_batch_size = 8
validation_batch_size = 8

# %%
# TODO1: Create batched data for DataLoader
# `collate_fn` is a function that defines how the data batch should be packed.
# This function will be called in the DataLoader to pack the data batch.

def collate_fn(batch):
    # TODO1-1: Implement the collate_fn function
    premises, hypotheses, relatedness_scores, entailment_judgments = zip(
        *[(item["premise"], item["hypothesis"], item["relatedness_score"], item["entailment_judgment"]) for item in batch]
    )
    
    # Tokenize the premises and hypotheses
    encodings = tokenizer(premises, hypotheses, truncation=True, padding=True, return_tensors="pt")
    
    # Convert relatedness scores and entailment judgments to tensors
    relatedness_scores = torch.tensor(relatedness_scores, dtype=torch.float)
    entailment_judgments = torch.tensor(entailment_judgments, dtype=torch.long)
    
    return encodings, relatedness_scores, entailment_judgments


# TODO1-2: Define your DataLoader
dl_train = DataLoader(SemevalDataset(split="train"), batch_size=train_batch_size, collate_fn=collate_fn)
dl_validation = DataLoader(SemevalDataset(split="validation"), batch_size=validation_batch_size, collate_fn=collate_fn)

# %%
class MultiLabelModel(torch.nn.Module):
    def __init__(self, bert_model_name="bert-base-uncased"):
        super(MultiLabelModel, self).__init__()
        # 使用 Hugging Face 的 BERT 模型
        self.bert = T.BertModel.from_pretrained(bert_model_name)
        
        # 線性層用於輸出相關性分數 (regression)
        self.regressor = torch.nn.Linear(self.bert.config.hidden_size, 1)
        
        # 線性層用於輸出推理判斷 (classification)
        self.classifier = torch.nn.Linear(self.bert.config.hidden_size, 3)  # 假設推理判斷有三個類別
        
    def forward(self, input_ids, attention_mask, token_type_ids=None):
        # 使用 BERT 模型進行前向傳播
        outputs = self.bert(input_ids=input_ids, attention_mask=attention_mask, token_type_ids=token_type_ids)
        
        # 獲取 [CLS] 標記的輸出（第0個位置）
        cls_output = outputs.last_hidden_state[:, 0, :]
        
        # 預測相關性分數
        relatedness_score = self.regressor(cls_output).squeeze(-1)  # 轉換形狀為 (batch_size,)
        
        # 預測推理判斷
        entailment_judgment = self.classifier(cls_output)  # 輸出形狀為 (batch_size, num_classes)
        
        return relatedness_score, entailment_judgment


# %%
# TODO2: Construct your model
class MultiLabelModel(torch.nn.Module):
    def __init__(self, bert_model_name="bert-base-uncased"):
        super(MultiLabelModel, self).__init__()
        # 使用 Hugging Face 的 BERT 模型
        self.bert = T.BertModel.from_pretrained(bert_model_name)
        
        # 線性層用於輸出相關性分數 (regression)
        self.regressor = torch.nn.Linear(self.bert.config.hidden_size, 1)
        
        # 線性層用於輸出推理判斷 (classification)
        self.classifier = torch.nn.Linear(self.bert.config.hidden_size, 3)  # 假設推理判斷有三個類別
        
    def forward(self, input_ids, attention_mask, token_type_ids=None):
        # 使用 BERT 模型進行前向傳播
        outputs = self.bert(input_ids=input_ids, attention_mask=attention_mask, token_type_ids=token_type_ids)
        
        # 獲取 [CLS] 標記的輸出（第0個位置）
        cls_output = outputs.last_hidden_state[:, 0, :]
        
        # 預測相關性分數
        relatedness_score = self.regressor(cls_output).squeeze(-1)  # 轉換形狀為 (batch_size,)
        
        # 預測推理判斷
        entailment_judgment = self.classifier(cls_output)  # 輸出形狀為 (batch_size, num_classes)
        
        return relatedness_score, entailment_judgment

# %%
model = MultiLabelModel().to(device)

# %%
# TODO3: Define your optimizer and loss function

# TODO3-1: Define your Optimizer
optimizer = AdamW(model.parameters(), lr=3e-5)

# TODO3-2: Define your loss functions (you should have two)
# 損失函數1：回歸任務使用 MSE 損失
regression_loss_fn = torch.nn.MSELoss()

# 損失函數2：分類任務使用 Cross-Entropy 損失
classification_loss_fn = torch.nn.CrossEntropyLoss()

# scoring functions
spc = SpearmanCorrCoef()
acc = Accuracy(task="multiclass", num_classes=3)
f1 = F1Score(task="multiclass", num_classes=3, average='macro')


# %%
for ep in range(epochs):
    pbar = tqdm(dl_train)
    pbar.set_description(f"Training epoch [{ep+1}/{epochs}]")
    model.train()
    
    for batch in pbar:
        # 清除上一步的梯度
        optimizer.zero_grad()
        
        # 提取輸入和標籤
        encodings, relatedness_scores, entailment_judgments = batch
        
        # 將數據移動到可用設備
        encodings = {k: v.to(device) for k, v in encodings.items()}
        relatedness_scores = relatedness_scores.to(device)
        entailment_judgments = entailment_judgments.to(device)
        
        # 前向傳播
        pred_relatedness, pred_entailment = model(**encodings)
        
        # 計算損失
        loss_relatedness = regression_loss_fn(pred_relatedness, relatedness_scores)
        loss_entailment = classification_loss_fn(pred_entailment, entailment_judgments)
        total_loss = loss_relatedness + loss_entailment  # 總損失為兩者相加
        
        # 反向傳播和優化
        total_loss.backward()
        optimizer.step()
        
        # 更新進度條的描述
        pbar.set_postfix(loss=total_loss.item())
    
    # 驗證階段
    pbar = tqdm(dl_validation)
    pbar.set_description(f"Validation epoch [{ep+1}/{epochs}]")
    model.eval()
    
    # 初始化計算指標的變量
    all_pred_relatedness = []
    all_true_relatedness = []
    all_pred_entailment = []
    all_true_entailment = []
    
    with torch.no_grad():
        for batch in pbar:
            encodings, relatedness_scores, entailment_judgments = batch
            
            # 將數據移動到可用設備
            encodings = {k: v.to(device) for k, v in encodings.items()}
            relatedness_scores = relatedness_scores.to(device)
            entailment_judgments = entailment_judgments.to(device)
            
            # 前向傳播
            pred_relatedness, pred_entailment = model(**encodings)
            
            # 收集預測和真實值
            all_pred_relatedness.extend(pred_relatedness.cpu().numpy())
            all_true_relatedness.extend(relatedness_scores.cpu().numpy())
            all_pred_entailment.extend(pred_entailment.argmax(dim=1).cpu().numpy())
            all_true_entailment.extend(entailment_judgments.cpu().numpy())
    
    # 計算評估指標
    spc_score = spc(torch.tensor(all_pred_relatedness), torch.tensor(all_true_relatedness))
    acc_score = acc(torch.tensor(all_pred_entailment), torch.tensor(all_true_entailment))
    f1_score = f1(torch.tensor(all_pred_entailment), torch.tensor(all_true_entailment))
    
    print(f"Spearman Corr: {spc_score}")
    print(f"Accuracy: {acc_score}")
    print(f"F1 Score: {f1_score}")
    
    # 儲存模型
    torch.save(model, f'./saved_models/ep{ep}.ckpt')


# %% [markdown]
# # Testing

# %%

# 載入存檔的模型
model = torch.load('./saved_models/ep1.ckpt')
model.eval()

# 修正 SemevalDataset 義類以支援 test 分割
class SemevalDataset(Dataset):
    def __init__(self, split="train") -> None:
        super().__init__()
        assert split in ["train", "validation", "test"]  # 增加 "test" 支援
        self.data = load_dataset(
            "sem_eval_2014_task_1", split=split, cache_dir="./cache/"
        ).to_list()

    def __getitem__(self, index):
        d = self.data[index]
        # 把中文標點替換掉
        for k in ["premise", "hypothesis"]:
            for tok in token_replacement:
                d[k] = d[k].replace(tok[0], tok[1])
        return d

    def __len__(self):
        return len(self.data)

# 準備測試資料集 DataLoader
dl_test = DataLoader(SemevalDataset(split="test"), batch_size=validation_batch_size, collate_fn=collate_fn)

# 初始化計算指標的變量
all_pred_relatedness = []
all_true_relatedness = []

# 將 SpearmanCorrCoef 引入
from torchmetrics import SpearmanCorrCoef
spc = SpearmanCorrCoef()

with torch.no_grad():
    for batch in tqdm(dl_test, desc="Testing"):
        encodings, relatedness_scores, entailment_judgments = batch
        
        # 移動數據到設備
        encodings = {k: v.to(device) for k, v in encodings.items()}
        relatedness_scores = relatedness_scores.to(device)
        
        # 前向傳播
        pred_relatedness, _ = model(**encodings)
        
        # 收集預測和真實值
        all_pred_relatedness.extend(pred_relatedness.cpu().numpy())
        all_true_relatedness.extend(relatedness_scores.cpu().numpy())

# 計算 Spearman correlation coefficient
spc_score = spc(torch.tensor(all_pred_relatedness), torch.tensor(all_true_relatedness))

# 輸出結果
print(f"Spearman Corr on Test Set: {spc_score}")

# 確認 Bonus 分數
if spc_score > 0.8:
    print("Bonus: 10%")
elif spc_score > 0.77:
    print("Bonus: 7%")
elif spc_score > 0.74:
    print("Bonus: 3%")
else:
    print("No Bonus")


