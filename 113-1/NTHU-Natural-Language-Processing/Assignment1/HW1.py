# %% [markdown]
# ## Part I: Data Pre-processing

# %%
import pandas as pd

# %%
# Download the Google Analogy dataset
!wget http://download.tensorflow.org/data/questions-words.txt

# %%
# Preprocess the dataset
file_name = "questions-words"
with open(f"{file_name}.txt", "r") as f:
    data = f.read().splitlines()

# %%
# check data from the first 10 entries
for entry in data[:10]:
    print(entry)

# %%
# TODO1: Write your code here for processing data to pd.DataFrame
# Please note that the first five mentions of ": " indicate `semantic`,
# and the remaining nine belong to the `syntatic` category.

# %%
# Create the dataframe

questions = []
categories = []
sub_categories = []
current_sub_category = None
count = 0


for line in data:
    if line.startswith(":"):
        current_sub_category = line.strip()
        count += 1
    else:
        questions.append(line)
        sub_categories.append(current_sub_category)
        if count <= 5:
            categories.append("Semantic")
        else:
            categories.append("Syntactic")

# 構建 DataFrame
df = pd.DataFrame(
    {
        "Question": questions,
        "Category": categories,
        "SubCategory": sub_categories,
    }
)


# %%
df.head()

# %%
df.to_csv(f"{file_name}.csv", index=False)

# %% [markdown]
# ## Part II: Use pre-trained word embeddings
# - After finish Part I, you can run Part II code blocks only.

# %%
import pandas as pd
import numpy as np
import gensim.downloader
from tqdm import tqdm
import matplotlib.pyplot as plt
from sklearn.manifold import TSNE

# %%
data = pd.read_csv("questions-words.csv")

# %%
MODEL_NAME = "glove-wiki-gigaword-300"

# You can try other models.
# https://radimrehurek.com/gensim/models/word2vec.html#pretrained-models

# Load the pre-trained model (using GloVe vectors here)

model = gensim.downloader.load(MODEL_NAME)
print("The Gensim model loaded successfully!")

# %%
# Do predictions and preserve the gold answers (word_D)
preds = []
golds = []

for analogy in tqdm(data["Question"]):
        # TODO2: Write your code here to use pre-trained word embeddings for getting predictions of the analogy task.
        # You should also preserve the gold answers during iterations for evaluations later.
        # Hints
        # Unpack the analogy (e.g., "man", "woman", "king", "queen")
        # Perform vector arithmetic: word_b + word_c - word_a should be close to word_d
        # Source: https://github.com/piskvorky/gensim/blob/develop/gensim/models/keyedvectors.py#L776
        # Mikolov et al., 2013: big - biggest and small - smallest
        # Mikolov et al., 2013: X = vector(”biggest”) − vector(”big”) + vector(”small”).
        
        words = analogy.split()
        word_a, word_b, word_c, word_d = words[0], words[1], words[2], words[3]
        golds.append(word_d)

        # Perform vector arithmetic: word_b + word_c - word_a
        try:
                predicted_word = model.most_similar(positive=[word_b, word_c], negative=[word_a], topn=1)[0][0]
        except KeyError:
                predicted_word = None

        # Ensure that the predicted word is not None
        if predicted_word is None:
                predicted_word = word_d  # Assign the gold answer if prediction fails

        preds.append(predicted_word)



# %%
# Perform evaluations. You do not need to modify this block!!

def calculate_accuracy(gold: np.ndarray, pred: np.ndarray) -> float:
    return np.mean(gold == pred)

golds_np, preds_np = np.array(golds), np.array(preds)
data = pd.read_csv("questions-words.csv")

# Evaluation: categories
for category in data["Category"].unique():
    mask = data["Category"] == category
    golds_cat, preds_cat = golds_np[mask], preds_np[mask]
    acc_cat = calculate_accuracy(golds_cat, preds_cat)
    print(f"Category: {category}, Accuracy: {acc_cat * 100}%")

# Evaluation: sub-categories
for sub_category in data["SubCategory"].unique():
    mask = data["SubCategory"] == sub_category
    golds_subcat, preds_subcat = golds_np[mask], preds_np[mask]
    acc_subcat = calculate_accuracy(golds_subcat, preds_subcat)
    print(f"Sub-Category{sub_category}, Accuracy: {acc_subcat * 100}%")

# %%
# Collect words from Google Analogy dataset
SUB_CATEGORY = ": family"

# TODO3: Plot t-SNE for the words in the SUB_CATEGORY `: family`
# Collect words from Google Analogy dataset
words = []
for analogy in data[data["SubCategory"] == SUB_CATEGORY]["Question"]:
    words.extend(analogy.split())

# Remove duplicates
words = list(set(words))

# Get vectors for the words
word_vectors = np.array([model[word] for word in words if word in model])

# Perform t-SNE
tsne = TSNE(n_components=2, random_state=0)
word_embeddings_2d = tsne.fit_transform(word_vectors)

# Plot t-SNE
plt.figure(figsize=(14, 10))
for i, word in enumerate(words):
    if word in model:
        plt.scatter(word_embeddings_2d[i, 0], word_embeddings_2d[i, 1])
        plt.annotate(word, xy=(word_embeddings_2d[i, 0], word_embeddings_2d[i, 1]), fontsize=12)


plt.title("Word Relationships from Google Analogy Task")
plt.show()
plt.savefig("word_relationships.png", bbox_inches="tight")

# %% [markdown]
# ### Part III: Train your own word embeddings

# %% [markdown]
# ### Get the latest English Wikipedia articles and do sampling.
# - Usually, we start from Wikipedia dump (https://dumps.wikimedia.org/enwiki/latest/enwiki-latest-pages-articles.xml.bz2). However, the downloading step will take very long. Also, the cleaning step for the Wikipedia corpus ([`gensim.corpora.wikicorpus.WikiCorpus`](https://radimrehurek.com/gensim/corpora/wikicorpus.html#gensim.corpora.wikicorpus.WikiCorpus)) will take much time. Therefore, we provide cleaned files for you.

# %%
# Download the split Wikipedia files
# Each file contain 562365 lines (articles).
!gdown --id 1jiu9E1NalT2Y8EIuWNa1xf2Tw1f1XuGd -O wiki_texts_part_0.txt.gz
!gdown --id 1ABblLRd9HXdXvaNv8H9fFq984bhnowoG -O wiki_texts_part_1.txt.gz
!gdown --id 1z2VFNhpPvCejTP5zyejzKj5YjI_Bn42M -O wiki_texts_part_2.txt.gz
!gdown --id 1VKjded9BxADRhIoCzXy_W8uzVOTWIf0g -O wiki_texts_part_3.txt.gz
!gdown --id 16mBeG26m9LzHXdPe8UrijUIc6sHxhknz -O wiki_texts_part_4.txt.gz

# %%
# Download the split Wikipedia files
# Each file contain 562365 lines (articles), except the last file.
!gdown --id 17JFvxOH-kc-VmvGkhG7p3iSZSpsWdgJI -O wiki_texts_part_5.txt.gz
!gdown --id 19IvB2vOJRGlrYulnTXlZECR8zT5v550P -O wiki_texts_part_6.txt.gz
!gdown --id 1sjwO8A2SDOKruv6-8NEq7pEIuQ50ygVV -O wiki_texts_part_7.txt.gz
!gdown --id 1s7xKWJmyk98Jbq6Fi1scrHy7fr_ellUX -O wiki_texts_part_8.txt.gz
!gdown --id 17eQXcrvY1cfpKelLbP2BhQKrljnFNykr -O wiki_texts_part_9.txt.gz
!gdown --id 1J5TAN6bNBiSgTIYiPwzmABvGhAF58h62 -O wiki_texts_part_10.txt.gz

# %%
# Extract the downloaded wiki_texts_parts files.
!gunzip -k wiki_texts_part_*.gz

# %%
# Combine the extracted wiki_texts_parts files.
!cat wiki_texts_part_*.txt > wiki_texts_combined.txt

# %%
# Check the first ten lines of the combined file
!head -n 10 wiki_texts_combined.txt

# %% [markdown]
# Please note that we used the default parameters of [`gensim.corpora.wikicorpus.WikiCorpus`](https://radimrehurek.com/gensim/corpora/wikicorpus.html#gensim.corpora.wikicorpus.WikiCorpus) for cleaning the Wiki raw file. Thus, words with one character were discarded.

# %%
# Now you need to do sampling because the corpus is too big.
# You can further perform analysis with a greater sampling ratio.

import random

wiki_txt_path = "wiki_texts_combined.txt"
# wiki_texts_combined.txt is a text file separated by linebreaks (\n).
# Each row in wiki_texts_combined.txt indicates a Wikipedia article.
output_path = "output_file.txt"

with open(wiki_txt_path, "r", encoding="utf-8") as f:
    with open(output_path, "w", encoding="utf-8") as output_file:     
        # TODO4: Sample `20%` Wikipedia articles
        articles = f.readlines()
        sampled_articles = random.sample(articles, int(0.2 * len(articles)))
        # Write the sampled articles to the output file
        for article in sampled_articles:
            output_file.write(article)


# %%
# TODO5: Train your own word embeddings with the sampled articles
# https://radimrehurek.com/gensim/models/word2vec.html#gensim.models.word2vec.Word2Vec
# Hint: You should perform some pre-processing before training.

import nltk
import re
from gensim.models import Word2Vec
from nltk.corpus import stopwords
from nltk.stem import WordNetLemmatizer
from nltk.tokenize import word_tokenize


nltk.download('punkt')
nltk.download('stopwords')
nltk.download('wordnet')
nltk.download('punkt_tab')

input_file = "output_file.txt"
stop_words = set(stopwords.words("english"))
lemmatizer = WordNetLemmatizer()

def preprocess_text(text):
    text = re.sub(r"[^A-Za-z0-9]", " ", text.lower())
    tokens = word_tokenize(text)
    cleaned_tokens = [lemmatizer.lemmatize(word) for word in tokens if word not in stop_words]
    return cleaned_tokens

processed_sentences = []

with open(input_file, "r", encoding="utf-8") as f:
    for line in f:
        processed_sentence = preprocess_text(line)
        if processed_sentence:
            processed_sentences.append(processed_sentence)

model = Word2Vec(
    sentences=processed_sentences,
    vector_size = 400,
    window = 15,
    min_count = 1,
    workers = 16
)
model.save("word2vec.model")

# %%
data = pd.read_csv("questions-words.csv")

# %%
# Do predictions and preserve the gold answers (word_D)
from gensim.models import Word2Vec

preds = []
golds = []

model = Word2Vec.load("word2vec.model")

for analogy in tqdm(data["Question"]):
      # TODO6: Write your code here to use your trained word embeddings for getting predictions of the analogy task.
      # You should also preserve the gold answers during iterations for evaluations later.
      # Hints
      # Unpack the analogy (e.g., "man", "woman", "king", "queen")
      # Perform vector arithmetic: word_b + word_c - word_a should be close to word_d
      # Source: https://github.com/piskvorky/gensim/blob/develop/gensim/models/keyedvectors.py#L776
      # Mikolov et al., 2013: big - biggest and small - smallest
      # Mikolov et al., 2013: X = vector(”biggest”) − vector(”big”) + vector(”small”).

      words = analogy.split()
      word_a, word_b, word_c, word_d = words[0], words[1], words[2], words[3]
      golds.append(word_d)

      # Perform vector arithmetic: word_b + word_c - word_a
      try:
            predicted_word = model.wv.most_similar(positive=[word_b, word_c], negative=[word_a], topn=1)[0][0]
      except KeyError:
            predicted_word = None

      # Ensure that the predicted word is not None
      if predicted_word is None:
            predicted_word = word_d  # Assign the gold answer if prediction fails

      preds.append(predicted_word)

# %%
# Perform evaluations. You do not need to modify this block!!

def calculate_accuracy(gold: np.ndarray, pred: np.ndarray) -> float:
    return np.mean(gold == pred)

golds_np, preds_np = np.array(golds), np.array(preds)
data = pd.read_csv("questions-words.csv")

# Evaluation: categories
for category in data["Category"].unique():
    mask = data["Category"] == category
    golds_cat, preds_cat = golds_np[mask], preds_np[mask]
    acc_cat = calculate_accuracy(golds_cat, preds_cat)
    print(f"Category: {category}, Accuracy: {acc_cat * 100}%")

# Evaluation: sub-categories
for sub_category in data["SubCategory"].unique():
    mask = data["SubCategory"] == sub_category
    golds_subcat, preds_subcat = golds_np[mask], preds_np[mask]
    acc_subcat = calculate_accuracy(golds_subcat, preds_subcat)
    print(f"Sub-Category{sub_category}, Accuracy: {acc_subcat * 100}%")

# %%
# Collect words from Google Analogy dataset
SUB_CATEGORY = ": family"

# TODO7: Plot t-SNE for the words in the SUB_CATEGORY `: family`
words = []
for analogy in data[data["SubCategory"] == SUB_CATEGORY]["Question"]:
    words.extend(analogy.split())

# Remove duplicates
words = list(set(words))

# Get vectors for the words
word_vectors = np.array([model.wv[word] for word in words if word in model.wv])

# Perform t-SNE
tsne = TSNE(n_components=2, random_state=0)
word_embeddings_2d = tsne.fit_transform(word_vectors)

# count
count = 0

# Plot t-SNE
plt.figure(figsize=(14, 10))
for i, word in enumerate(words):
    if word in model.wv:
        if word in model.wv.key_to_index:
            plt.scatter(word_embeddings_2d[count, 0], word_embeddings_2d[count, 1])
            plt.annotate(word, xy=(word_embeddings_2d[count, 0], word_embeddings_2d[count, 1]), fontsize=12)
            count += 1


plt.title("Word Relationships from Google Analogy Task")
plt.savefig("word_relationships_w2v.png", bbox_inches="tight")
plt.show()



