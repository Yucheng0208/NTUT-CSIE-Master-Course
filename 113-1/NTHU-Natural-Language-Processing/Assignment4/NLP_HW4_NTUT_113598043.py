# -*- coding: utf-8 -*-
"""assignment4_RAG.ipynb

Automatically generated by Colab.

Original file is located at
    https://colab.research.google.com/drive/1Boa2KHjoaOaiJs4aIlvQO7GtVGGIrOnf

# RAG using Langchain

## Packages loading & import
"""

!nvidia-smi

!pip install langchain
!pip install langchain_community
!pip install langchain_huggingface
!pip install langchain_text_splitters
!pip install langchain_chroma
!pip install rank-bm25
!pip install huggingface_hub

import os
import json
import bs4
import nltk
import torch
import pickle
import numpy as np

# from pyserini.index import IndexWriter
# from pyserini.search import SimpleSearcher
from numpy.linalg import norm
from rank_bm25 import BM25Okapi
from nltk.tokenize import word_tokenize

from langchain_community.llms import Ollama
from langchain.chains.combine_documents import create_stuff_documents_chain
from langchain.chains import create_retrieval_chain
from langchain.vectorstores import Chroma
from sentence_transformers import SentenceTransformer
from langchain_huggingface import HuggingFaceEmbeddings
from langchain_community.embeddings import JinaEmbeddings
from langchain.text_splitter import RecursiveCharacterTextSplitter, TokenTextSplitter
from langchain.docstore.document import Document
from langchain_core.prompts import ChatPromptTemplate
from langchain_community.document_loaders import WebBaseLoader
from transformers import AutoModel, AutoModelForCausalLM, AutoTokenizer

from tqdm import tqdm

nltk.download('punkt')
nltk.download('punkt_tab')

"""## Hugging face login
- Please apply the model first: https://huggingface.co/meta-llama/Llama-3.2-1B-Instruct
- If you haven't been granted access to this model, you can use other LLM model that doesn't have to apply.
- You must save the hf token otherwise you need to regenrate the token everytime.
- When using Ollama, no login is required to access and utilize the llama model.
"""

from huggingface_hub import login

hf_token = "hf_" # write your token on here
login(token=hf_token, add_to_git_credential=True)

!huggingface-cli whoami

"""## TODO1: Set up the environment of Ollama

### Introduction to Ollama
- Ollama is a platform designed for running and managing large language models (LLMs) directly **on local devices**, providing a balance between performance, privacy, and control.
- There are also other tools support users to manage LLM on local devices and accelerate it like *vllm*, *Llamafile*, *GPT4ALL*...etc.

### Launch colabxterm
"""

# Commented out IPython magic to ensure Python compatibility.
# TODO1-1: You should install colab-xterm and launch it.
# Write your commands here.
!pip install colab-xterm
# %load_ext colabxterm

# Commented out IPython magic to ensure Python compatibility.
# TODO1-2: You should install Ollama.
# You may need root privileges if you use a local machine instead of Colab.
!pip install ollama
# %load_ext ollama

# Commented out IPython magic to ensure Python compatibility.
# %xterm
# curl -fsSL https://ollama.com/install.sh | sh
# ollama serve

# Commented out IPython magic to ensure Python compatibility.
# TODO1-3: Pull Llama3.2:1b via Ollama and start the Ollama service in the xterm
# Write your commands in the xterm
# %xterm
# ollama pull llama3.2:1b / llama3.2:3b

"""## Ollama testing
You can test your Ollama status with the following cells.
"""

# Setting up the model that this tutorial will use
#MODEL = "llama3.2:1b" # https://ollama.com/library/llama3.2:3b
MODEL = "llama3.2:3b"
EMBED_MODEL = "jinaai/jina-embeddings-v2-base-en"

# Initialize an instance of the Ollama model
llm = Ollama(model=MODEL)
# Invoke the model to generate responses
response = llm.invoke("What is the capital of Taiwan?")
print(response)

"""## Build a simple RAG system by using LangChain

### TODO2: Load the cat-facts dataset and prepare the retrieval database
"""

!wget https://huggingface.co/ngxson/demo_simple_rag_py/resolve/main/cat-facts.txt

# TODO2-1: Load the cat-facts dataset (as `refs`, which is a list of strings for all the cat facts)
# Write your code here
with open("cat-facts.txt", "r") as f:
    refs = f.readlines()
    refs = [ref.strip() for ref in refs]
    print(refs[:5])
    print(len(refs))

from langchain_core.documents import Document
docs = [Document(page_content=doc, metadata={"id": i}) for i, doc in enumerate(refs)]

# Create an embedding model
model_kwargs = {'trust_remote_code': True}
encode_kwargs = {'normalize_embeddings': False}
embeddings_model = HuggingFaceEmbeddings(
    model_name=EMBED_MODEL,
    model_kwargs=model_kwargs,
    encode_kwargs=encode_kwargs
)

# TODO2-2: Prepare the retrieval database
# You should create a Chroma vector store.
# search_type can be “similarity” (default), “mmr”, or “similarity_score_threshold”
vector_store = Chroma.from_documents(
    # Write your code here
    documents=docs,
    embedding=embeddings_model,
    persist_directory="./chroma_db"
)
retriever = vector_store.as_retriever(
    # Write your code here
    search_type="mmr",
    #search_kwargs={"k": 3}
)

"""### Prompt setting"""

# TODO3: Set up the `system_prompt` and configure the prompt.
system_prompt = (
    "Use the given context to answer the question."
    "If you don't know the answer, say you don't know."
    "Use three sentence maximum and keep the answer: concise."
    "Context: {context}"
)
# Write your code here
prompt = ChatPromptTemplate.from_messages(
    [
        ("system", system_prompt),
        ("human", "{input}"),
    ]
)

"""- For the vectorspace, the common algorithm would be used like Faiss, Chroma...(https://python.langchain.com/docs/integrations/vectorstores/) to deal with the extreme huge database."""

# TODO4: Build and run the RAG system
# TODO4-1: Load the QA chain
# You should create a chain for passing a list of Documents to a model.
question_answer_chain = create_stuff_documents_chain(llm=llm, prompt=prompt)
# Write your code here

# TODO4-2: Create retrieval chain
# You should create retrieval chain that retrieves documents and then passes them on.
chain = create_retrieval_chain(retriever=retriever, combine_docs_chain=question_answer_chain) # Write your code here

# Question (queries) and answer pairs
# Please do not modify this cell.
queries = [
    "How much of a day do cats spend sleeping on average?",
    "What is the technical term for a cat's hairball?",
    "What do scientists believe caused cats to lose their sweet tooth?",
    "What is the top speed a cat can travel over short distances?",
    "What is the name of the organ in a cat's mouth that helps it smell?",
    "Which wildcat is considered the ancestor of all domestic cats?",
    "What is the group term for cats?",
    "How many different sounds can cats make?",
    "What is the name of the first cat in space?",
    "How many toes does a cat have on its back paws?"
]
answers = [
    "2/3",
    "Bezoar",
    "a mutation in a key taste receptor",
    ["31 mph", "49 km"],
    "Jacobson’s organ",
    "the African Wild Cat",
    "clowder",
    "100",
    ["Felicette", "Astrocat"],
    "four",
]

counts = 0
for i, query in enumerate(queries):
    # TODO4-3: Run the RAG system
    response = chain.invoke({"input": query})  # Write your code here
    print(f"Query: {query}\nResponse: {response['answer']}\n")
    # The following lines perform evaluations.
    # if the answer shows up in your response, the response is considered correct.
    if type(answers[i]) == list:
        for answer in answers[i]:
            if answer.lower() in response['answer'].lower():
                counts += 1
                break
    else:
        if answers[i].lower() in response['answer'].lower():
            counts += 1
            print("Status: Correct!")

# TODO5: Improve to let the LLM correctly answer the ten questions.
print(f"Correct numbers: {counts}")