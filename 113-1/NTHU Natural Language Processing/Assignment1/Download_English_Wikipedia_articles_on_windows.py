import os
import requests
import gzip
import shutil

# 定義檔案 ID 和本地儲存路徑
file_ids = [
    "1jiu9E1NalT2Y8EIuWNa1xf2Tw1f1XuGd",
    "1ABblLRd9HXdXvaNv8H9fFq984bhnowoG",
    "1z2VFNhpPvCejTP5zyejzKj5YjI_Bn42M",
    "1VKjded9BxADRhIoCzXy_W8uzVOTWIf0g",
    "16mBeG26m9LzHXdPe8UrijUIc6sHxhknz",
    "17JFvxOH-kc-VmvGkhG7p3iSZSpsWdgJI",
    "19IvB2vOJRGlrYulnTXlZECR8zT5v550P",
    "1sjwO8A2SDOKruv6-8NEq7pEIuQ50ygVV",
    "1s7xKWJmyk98Jbq6Fi1scrHy7fr_ellUX",
    "17eQXcrvY1cfpKelLbP2BhQKrljnFNykr",
    "1J5TAN6bNBiSgTIYiPwzmABvGhAF58h62"
]
download_dir = "downloads"
combined_file = "wiki_texts_combined.txt"

# 建立下載目錄
os.makedirs(download_dir, exist_ok=True)

# 下載檔案
for i, file_id in enumerate(file_ids):
    url = f"https://drive.google.com/uc?id={file_id}"
    file_path = os.path.join(download_dir, f"wiki_texts_part_{i}.txt.gz")
    response = requests.get(url, stream=True)
    with open(file_path, "wb") as file:
        shutil.copyfileobj(response.raw, file)
    print(f"Downloaded {file_path}")

# 解壓縮並轉換為文字格式
text_files = []
for i in range(len(file_ids)):
    gz_file_path = os.path.join(download_dir, f"wiki_texts_part_{i}.txt.gz")
    txt_file_path = os.path.join(download_dir, f"wiki_texts_part_{i}.txt")
    with gzip.open(gz_file_path, "rt", encoding="utf-8") as gz_file, open(txt_file_path, "w", encoding="utf-8") as txt_file:
        txt_file.write(gz_file.read())
    text_files.append(txt_file_path)
    print(f"Converted {gz_file_path} to {txt_file_path}")=

# 合併所有文字檔案
with open(combined_file, "w", encoding="utf-8") as outfile:
    for txt_file_path in text_files:
        with open(txt_file_path, "r", encoding="utf-8") as infile:
            outfile.write(infile.read())
            outfile.write("\n")
    print(f"Combined all text files into {combined_file}")

# 刪除中間產生的 .txt 和 .gz 檔案
for i in range(len(file_ids)):
    os.remove(os.path.join(download_dir, f"wiki_texts_part_{i}.txt"))
    os.remove(os.path.join(download_dir, f"wiki_texts_part_{i}.txt.gz"))
    print(f"Deleted wiki_texts_part_{i}.txt and wiki_texts_part_{i}.txt.gz")