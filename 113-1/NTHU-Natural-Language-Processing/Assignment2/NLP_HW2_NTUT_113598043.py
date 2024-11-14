import pandas as pd
import torch
import os

data_path = './data'

df_train = pd.read_csv(os.path.join(data_path, 'arithmetic_train.csv'))
df_eval = pd.read_csv(os.path.join(data_path, 'arithmetic_eval.csv'))
df_train.head()

df_train = df_train.iloc[:, 1:]
df_eval = df_eval.iloc[:, 1:]

# transform the input data to string
df_train['tgt'] = df_train['tgt'].apply(lambda x: str(x))
df_train['src'] = df_train['src'].add(df_train['tgt'])
df_train['len'] = df_train['src'].apply(lambda x: len(x))

df_eval['tgt'] = df_eval['tgt'].apply(lambda x: str(x))
df_eval['src'] = df_eval['src'].add(df_eval['tgt'])
df_eval['len'] = df_eval['src'].apply(lambda x: len(x))

char_to_id = {}
id_to_char = {}

# Build the dictionary and assign IDs to each token in the training dataset
# The dictionary should include <eos> and <pad>
# char_to_id is used to convert characters to IDs, while id_to_char does the reverse
special_tokens = ['<pad>', '<eos>'] 

# Include all characters in src and tgt
all_chars = set(''.join(df_train['src']) + ''.join(df_train['tgt']) + ''.join(df_eval['src']) + ''.join(df_eval['tgt']))

# Separate digits and symbols
digits = sorted([char for char in all_chars if char.isdigit()])
symbols = sorted([char for char in all_chars if not char.isdigit()])

# Assign IDs to special tokens
char_to_id = {'<pad>': 0, '<eos>': 1}

# Assign IDs to other characters
char_to_id.update({char: idx + 2 for idx, char in enumerate(digits + symbols)})
id_to_char = {idx: char for char, idx in char_to_id.items()}

# Get the vocabulary size
vocab_size = len(char_to_id)
print('Vocab size: {}'.format(vocab_size))

# Display the dictionary
char_to_id_df = pd.DataFrame(list(char_to_id.items()), columns=['ID', 'Character'])
print(char_to_id_df)

# Display the first training sample
df_train['src'].iloc[0]

# Display the first training sample after tokenization
df_train = df_train[['src', 'tgt', 'len']]
df_train['char_id_list'] = df_train['src'].apply(lambda x: [char_to_id[char] for char in x] + [char_to_id['<eos>']])
df_train['label_id_list'] = df_train.apply(lambda row: [char_to_id['<pad>']] * (row['len'] - len(row['tgt'])) + [char_to_id[char] for char in row['tgt']] + [char_to_id['<eos>']], axis=1)
df_train.head()

# Display the first evaluation sample after tokenization
df_eval = df_eval[['src', 'tgt', 'len']]
df_eval['char_id_list'] = df_eval['src'].apply(lambda x: [char_to_id[char] for char in x] + [char_to_id['<eos>']])
df_eval['label_id_list'] = df_eval.apply(lambda row: [char_to_id['<pad>']] * (row['len'] - len(row['tgt'])) + [char_to_id[char] for char in row['tgt']] + [char_to_id['<eos>']], axis=1)
df_eval.head()

# Hyperparameters
batch_size = 64
epochs = 10
embed_dim = 256
hidden_dim = 256
lr = 0.001
grad_clip = 1

# # Data Batching
class Dataset(torch.utils.data.Dataset):
    def __init__(self, sequences):
        self.sequences = sequences
    
    def __len__(self):
        # return the amount of data
        return len(self.sequences)
    
    def __getitem__(self, index):
        # Extract the input data x and the ground truth y from the data
        x = self.sequences.iloc[index, 0][:-1]
        y = self.sequences.iloc[index, 1][1:]
        return x, y

# collate function, used to build dataloader
def collate_fn(batch):
    batch_x = [torch.tensor(data[0]) for data in batch]
    batch_y = [torch.tensor(data[1]) for data in batch]
    batch_x_lens = torch.LongTensor([len(x) for x in batch_x])
    batch_y_lens = torch.LongTensor([len(y) for y in batch_y])
    
    # Pad the input sequence
    pad_batch_x = torch.nn.utils.rnn.pad_sequence(batch_x,
                                                  batch_first=True,
                                                  padding_value=char_to_id['<pad>'])
    
    pad_batch_y = torch.nn.utils.rnn.pad_sequence(batch_y,
                                                  batch_first=True,
                                                  padding_value=char_to_id['<pad>'])
    
    return pad_batch_x, pad_batch_y, batch_x_lens, batch_y_lens

# Create the dataset
ds_train = Dataset(df_train[['char_id_list', 'label_id_list']])
ds_eval = Dataset(df_eval[['char_id_list', 'label_id_list']])

# Build dataloader of train set and eval set, collate_fn is the collate function
dl_train = torch.utils.data.DataLoader(ds_train, batch_size=batch_size, collate_fn=collate_fn, shuffle=True)
dl_eval = torch.utils.data.DataLoader(ds_eval, batch_size=batch_size, collate_fn=collate_fn, shuffle=False)


# LSTM model
class CharRNN(torch.nn.Module):
    def __init__(self, vocab_size, embed_dim, hidden_dim):
        super(CharRNN, self).__init__()
        
        self.embedding = torch.nn.Embedding(num_embeddings=vocab_size,
                                            embedding_dim=embed_dim,
                                            padding_idx=char_to_id['<pad>'])
        
        self.rnn_layer1 = torch.nn.LSTM(input_size=embed_dim,
                                        hidden_size=hidden_dim,
                                        batch_first=True)
        
        self.dropout = torch.nn.Dropout(p=0.3)

        self.rnn_layer2 = torch.nn.LSTM(input_size=hidden_dim,
                                        hidden_size=hidden_dim,
                                        batch_first=True)
        
        self.linear = torch.nn.Sequential(
            torch.nn.Linear(in_features=hidden_dim, out_features=hidden_dim),
            torch.nn.LeakyReLU(),
            torch.nn.Linear(in_features=hidden_dim, out_features=vocab_size)
        )
        
    def forward(self, batch_x, batch_x_lens):
        return self.encoder(batch_x, batch_x_lens)
    
    # The forward pass of the model
    def encoder(self, batch_x, batch_x_lens):
        # Embed the input sequence
        batch_x = self.embedding(batch_x)
        
        # Pack the sequence
        packed_input = torch.nn.utils.rnn.pack_padded_sequence(batch_x,
                                                            batch_x_lens,
                                                            batch_first=True,
                                                            enforce_sorted=False)
        
        # Pass through the first LSTM layer
        packed_output, _ = self.rnn_layer1(packed_input)
        
        # Pass through the second LSTM layer
        packed_output, _ = self.rnn_layer2(packed_output)
        
        # Unpack the sequence
        batch_x, _ = torch.nn.utils.rnn.pad_packed_sequence(packed_output,
                                                            batch_first=True)
        
        # Pass through the linear layer
        batch_x = self.linear(batch_x)
        
        return batch_x
    
    # Generation function
    def generator(self, start_char, max_len=50):
        
        # Convert the start character to ID
        char_list = [char_to_id[c] for c in start_char]
        input_tensor = torch.tensor([char_list], dtype=torch.long).to(next(self.parameters()).device)
        
        # Initialize hidden states
        hidden_state1 = (torch.zeros(1, 1, self.rnn_layer1.hidden_size).to(next(self.parameters()).device),
                         torch.zeros(1, 1, self.rnn_layer1.hidden_size).to(next(self.parameters()).device))
        hidden_state2 = (torch.zeros(1, 1, self.rnn_layer2.hidden_size).to(next(self.parameters()).device),
                         torch.zeros(1, 1, self.rnn_layer2.hidden_size).to(next(self.parameters()).device))
        
        while len(char_list) < max_len: 
            # Embed the input tensor
            input_tensor = torch.tensor([[char_list[-1]]], dtype=torch.long).to(next(self.parameters()).device)
            embedded = self.embedding(input_tensor)

            # Pass through the first LSTM layer
            output, hidden_state1 = self.rnn_layer1(embedded, hidden_state1)
            output, hidden_state2 = self.rnn_layer2(output, hidden_state2)
            
            # Pass through the linear layer
            output = self.linear(output)
            y = output[:, -1, :]
            
            # Get the next character
            next_char = torch.argmax(y, dim=1).item()
            
            # If the next character is <eos>, stop
            if next_char == char_to_id['<eos>']:
                break
            
            # Append the next character to the list
            char_list.append(next_char)
        
        # Convert the ID list to a string
        return ''.join([id_to_char[ch_id] for ch_id in char_list])

# Set the random seed
torch.manual_seed(2)

# Check if GPU is available
device = torch.device("cuda" if torch.cuda.is_available() else "cpu")

if device.type == 'cuda': print("GPU is ready")
else: print("CPU is ready")

# Create the model
model = CharRNN(vocab_size,
                embed_dim,
                hidden_dim)

# Display the model
import torch.nn.functional as F

# Define the loss function and optimizer
criterion = torch.nn.CrossEntropyLoss(ignore_index=char_to_id['<pad>'])

optimizer = torch.optim.AdamW(model.parameters(), lr=lr, weight_decay=0.01)

# Training and evaluation
from tqdm import tqdm
from copy import deepcopy

# Move the model to the device
model = model.to(device)
best_em = 0.0
i = 0

# Training and evaluation
for epoch in range(1, epochs+1):
    # Training phase
    model.train()
    bar = tqdm(dl_train, desc=f"Train epoch {epoch}")

    # Training loop
    for batch_x, batch_y, batch_x_lens, batch_y_lens in bar:

        optimizer.zero_grad()

        batch_pred_y = model(batch_x.to(device), batch_x_lens)
        pred_y_view = batch_pred_y.view(-1, vocab_size)
        y_view = batch_y.view(-1).to(device)

        # Calculate the loss
        loss = criterion(pred_y_view, y_view)
        loss.backward()

        # Clip the gradient
        torch.nn.utils.clip_grad_value_(model.parameters(), 0.5)
        optimizer.step()

        # Update the progress bar
        i += 1
        if i % 50 == 0:
            bar.set_postfix(loss=loss.item())

    # Validation phase
    bar = tqdm(dl_eval, desc=f"Validation epoch {epoch}")
    matched = 0
    total = 0
    model.eval()

    # Validation loop
    for batch_x, batch_y, batch_x_lens, batch_y_lens in bar:
        batch_x = batch_x.to(device)
        batch_y = batch_y.to(device)

        with torch.no_grad():

            # Get the model predictions
            predictions = model(batch_x, batch_x_lens)

            # Get the predicted sequence
            predicted_ids = torch.argmax(predictions, dim=-1)

            # Calculate the exact match
            non_padding_mask = batch_y != 0
            
            # Check if the predicted sequence is correct
            matches = (predicted_ids == batch_y) | ~non_padding_mask

            # Check if all the characters in the sequence are correct
            matched += torch.all(matches, dim=1).sum().item()

            # Count the total number of sequences
            total += batch_y.size(0)

    # Calculate the exact match
    em = matched / total if total > 0 else 0
    print(f"Exact Match (EM) at epoch {epoch}: {em}")

    # Save the best model
    if em > best_em:
        best_em = em
        best_model = deepcopy(model)

# Move the best model to the CPU
model = model.to('cpu')
print(" ".join(model.generator('1+1=')))


