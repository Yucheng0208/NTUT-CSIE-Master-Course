# Google Cloud Skills Boost Google Cloud

- [Google Cloud Skills Boost Google Cloud](#google-cloud-skills-boost-google-cloud)
  - [02 Google Cloud Fundamentals: Core Infrastructure](#02-google-cloud-fundamentals-core-infrastructure)
    - [Introducing Google Cloud](#introducing-google-cloud)
    - [Resources and Access in the Cloud](#resources-and-access-in-the-cloud)
    - [Getting Started with VPC Networking and Google Compute Engine](#getting-started-with-vpc-networking-and-google-compute-engine)
    - [Storage in the Cloud](#storage-in-the-cloud)
      - [Containers in the Cloud](#containers-in-the-cloud)
    - [Applications in the Cloud](#applications-in-the-cloud)
    - [Prompt Engineering](#prompt-engineering)
  - [03 Essential Google Cloud Infrastructure: Foundation](#03-essential-google-cloud-infrastructure-foundation)
    - [Interacting with Google Cloud](#interacting-with-google-cloud)
    - [Virtual Networks](#virtual-networks)
      - [Virtual Machines](#virtual-machines)
  - [Disclaimer](#disclaimer)

## 02 Google Cloud Fundamentals: Core Infrastructure

### Introducing Google Cloud

1. What type of cloud computing service lets you bind your application code to libraries that give access to the infrastructure your application needs?
   - [ ] Virtualized data centers
   - [X] Platform as a service
   - [ ] Infrastructure as a service
   - [ ] Software as a service
> 說明：平台即服務（PaaS）讓應用程式綁定基礎設施所需的函式庫。

2. Why might a Google Cloud customer use resources in several regions around the world?
    - [ ] To improve security
    - [ ] To offer localized application versions in different regions
    - [ ] To earn discounts
    - [X] To bring their applications closer to users around the world, and for improved fault tolerance 
> 說明：把應用程式靠近全球使用者，並提升容錯性。

3. What is the primary benefit to a Google Cloud customer of using resources in several zones within a region?
    - [ ] For getting discounts on other zones
    - [X] For improved fault tolerance
    - [ ] For expanding services to customers in new areas
    - [ ] For better performance
> 說明：使用多個可用區提升故障容忍度。

### Resources and Access in the Cloud

1. Choose the correct completion: Services and APIs are enabled on a per-________ basis.
   - [ ] Organization
   - [X] Project
   - [ ] Billing account
   - [ ] Folder
> 說明：服務與 API 是以專案（Project）為單位啟用的。

2. Which of these values is globally unique, permanent, and unchangeable, but can be modified by the customer during creation?
    - [ ] The project name
    - [ ] The project's billing credit-card number
    - [ ] The project number
    - [X] The project ID 
> 說明：專案 ID 在建立時可設定，但之後不能更改，且全球唯一。

3. Order these IAM role types from broadest to finest-grained.
    - [ ] Predefined roles, custom roles, basic roles
    - [X] Basic roles, predefined roles, custom roles
    - [ ] Custom roles, predefined roles, basic roles
> 說明：IAM 權限範圍從大到小排序：基本角色 → 預設角色 → 自訂角色。

### Getting Started with VPC Networking and Google Compute Engine

1. Which firewall rule allows the ping to mynet-r2-vm's external IP address?
    - [ ] mynetwork-allow-ssh
    - [X] mynetwork-allow-icmp
    - [ ] mynetwork-allow-rdp
    - [ ] mynetwork-allow-custom
> 說明：允許 ICMP（ping）流量的防火牆規則。

2. How does Cloud Load Balancing allow you to balance HTTP-based traffic?
   - [ ] Across multiple physical machines in a single data center.
   - [ ] Across multiple virtual machine instances in a single Compute Engine region.
   - [X] Across multiple Compute Engine regions.
   - [ ] Across multiple Google Cloud Platform services.
> 說明：HTTP 流量可跨多個 Compute Engine 區域進行負載平衡。

3. For which of these interconnect options is a Service Level Agreement available?
    - [ ] Standard Network Tier
    - [ ] Direct Peering
    - [X] Dedicated Interconnect
    - [ ] Carrier Peering
> 說明：專線互連（Dedicated Interconnect）提供服務水準協議（SLA）。

### Storage in the Cloud

1. What is the correct use case for Cloud Storage?
    - [X] Cloud Storage is well suited to providing durable and highly available object storage.
    - [ ] Cloud Storage is well suited to providing RDBMS services.
    - [ ] Cloud Storage is well suited to providing data warehousing services.
    - [ ] Cloud Storage is well suited to providing the root file system of a Linux virtual machine.
> 說明：Cloud Storage 適合用來提供持久且高可用性的物件儲存。

2. Why would a customer consider the Coldline storage class?
    - [X] To save money on storing frequently accessed data.
    - [ ] To improve security.
    - [ ] To use the Coldline Storage API.
    - [ ] To save money on storing infrequently accessed data.
> 說明：Coldline 適合儲存不常存取的資料，能節省成本。

3. Which relational database service can scale to higher database sizes?
    - [X] Spanner
    - [ ] Firestore
    - [ ] Bigtable
    - [ ] Cloud SQL
> 說明：Spanner 可以支援更大規模的資料庫。

#### Containers in the Cloud

1. What is a Kubernetes pod?
    - [ ] A group of nodes
    - [ ] A group of clusters
    - [X] A group of containers
    - [ ] A group of VMs
> 說明：Pod 是一組容器。

2. Where do the resources used to build Google Kubernetes Engine clusters come from?
   - [ ] Bare-metal servers
   - [ ] App Engine
   - [X] Compute Engine
   - [ ] Cloud Storage
> 說明：GKE 叢集的資源來自 Compute Engine。

### Applications in the Cloud


1. Which of these statements about Cloud Run functions are correct? Select three. 'Cloud Run functions:'
**Multiple Select**
    - [X] Is a scalable functions-as-a-service platform.
    - [X] Is integrated with Cloud Logging.
    - [ ] Require servers or VMs to be provisioned.
    - [X] Can be used to extend Cloud services.
    - [ ] Can only be invoked by sending HTTP requests.
> 說明：Cloud Run Functions 是可擴展的平台，整合了 Cloud Logging，也可延伸其他雲端服務。

2. Why might a Google Cloud customer choose to use Cloud Run functions?
    - [X] Their application contains event-driven code that they don't want to provision compute resources for.
    - [ ] Their application has a legacy monolithic structure that they want to separate into microservices.
    - [ ] Cloud Run functions is a free service for hosting compute operations.
    - [ ] Cloud Run functions is the primary way to run C++ applications in Google Cloud.
> 說明：如果應用程式包含事件驅動的程式碼，又不想管理運算資源，適合使用 Cloud Run Functions。

3. Which scenario is best suited for using Cloud Run instead of Cloud Run functions?
   - [ ] Sending an email notification whenever a new document is added to a specific folder in Cloud Storage.
   - [ ] Resizing images on demand when requested by a user through a web interface.
   - [X] Hosting a dynamic web application that allows users to upload and share photos.
   - [ ] Generating thumbnails for images uploaded to a Cloud Storage bucket.
> 說明：如果要承載動態網頁應用，適合使用 Cloud Run。

### Prompt Engineering

1. Which of the following is not a good practice for constructing prompts?
    - [ ] Avoid ambiguity.
    - [ ] Vary your prompts.
    - [X] Include long and detailed sentences.
    - [ ] Keep it short and simple.
> 說明：編寫提示（Prompt）時，長且複雜的句子不是好習慣，應避免。

2. Which one of the following is not a type of prompt?
   - [ ] Zero-shot
   - [ ] One-shot
   - [X] Role prompt
   - [ ] Two-shot
> 說明：在常見的提示類型中，沒有「角色提示（Role prompt）」這個分類。

3. How can you avoid hallucinations when using a large language model?
    - [X] Train the model on a clean, curated dataset.
    - [ ] Train the LLM on a smaller dataset.
    - [ ] Do not add any context to your prompts.
    - [ ] Train the model to operate with very few constraints.
> 說明：使用乾淨且經過整理的資料集訓練模型，可以減少產生虛構內容（幻覺現象）。

4. Generative AI is a type of artificial intelligence that can ____.
   - [ ] Perform complex calculations and mathematical operations.
   - [ ] Generate responses on real-time data.
   - [ ] Make predictions about future events.
   - [X] Generate text, images, or other data using generative models.
> 說明：生成式 AI 是一種可以利用生成模型創造文字、影像或其他資料的人工智慧。

## 03 Essential Google Cloud Infrastructure: Foundation

### Interacting with Google Cloud

1. To create a persistent state in Cloud Shell, which file would you configure?
    - [ ] .my_variables
    - [X] .bashrc
    - [ ] .config
    - [ ] .profile
> 說明： 在 Cloud Shell 中設定 .bashrc 可保存個人環境設定。

2. What is the difference between the Google Cloud Console and Cloud Shell?
   - [ ] The Cloud Console is a command-line tool, while Cloud Shell is a graphical user interface
   - [ ] Cloud Shell is a locally installed tool, while the Cloud Console is a temporary virtual machine.
   - [ ] There is no difference as these tools are 100% identical.
   - [X] Cloud Shell is a command-line tool, while the Cloud Console is a graphical user interface
> 說明：Cloud Shell 是指令列工具；Cloud Console 是圖形化介面。

3. Which of the following does not allow you to interact with Google Cloud?
    - [X] Cloud Explorer
    - [ ] REST-based API
    - [ ] Cloud Shell
    - [ ] Google Cloud Console
> 說明：Cloud Explorer 不是用來與 GCP 互動的正式工具。

### Virtual Networks

1. Without a VPC network, you cannot create VM instances, containers, or App Engine applications.
    - [X] True
    - [ ] False
> 說明：沒有 VPC 網路就無法建立虛擬機、容器或 App Engine。



2. Which firewall rule allows the ping to mynet-notus-vm's external IP address?
    - [ ] mynetwork-allow-ssh
    - [X] mynetwork-allow-icmp
    - [ ] mynetwork-allow-custom
    - [ ] mynetwork-allow-rdp
> 說明：允許 ICMP（例如 ping）流量的防火牆規則。

3. Which instances should you be able to ping from mynet-us-vm using internal IP addresses?
**Multiple Select**
- [X] mynet-notus-vm
- [ ] managementnet-us-vm
- [X] privatenet-us-vm
> 說明：從 mynet-us-vm 內部 IP 可以 ping 到 mynot-notus-vm 與 privatenet-us-vm。

4. Did the command prompt change to `@vm-internal`?
    - [X] True
    - [ ] False
> 說明：提示符號應該會變成 @vm-internal。

5. Currently, which of your VM instances can access the image from your bucket?
   - [ ] Cloud Shell
   - [X] vm-internal
> 說明：目前只有 vm-internal 可以存取 bucket 中的圖片。

6. What are the three types of networks offered in Google Cloud?
   - [ ] Gigabit network, 10 gigabit network, and 100 gigabit network
   - [ ] IPv4 unicast network, IPv4 multicast network, IPv6 network
   - [ ] Zonal, regional, and global
   - [X] Default network, auto network, and custom network.
> 說明：Google Cloud 提供：預設網路、自動網路、客製網路。

7. What is one benefit of applying firewall rules by tag rather than by address?
   - [ ] Tags on firewall rules control which ephemeral IP addresses VMs will receive.
   - [X] When a VM is created with a matching tag, the firewall rules apply irrespective of the IP address it is assigned.
   - [ ] Tags in network traffic help with network sniffing.
   - [ ] Tags help organizations track firewall billing.
> 說明：Google Cloud 提供：預設網路、自動網路、客製網路。

8. In Google Cloud, what is the minimum number of IP addresses that a VM instance needs?
   - [X] One: Only an internal IP address
   - [ ] Two: One internal and one external IP address
   - [ ] Three: One internal, one external and one alias IP address
> 說明：虛擬機至少需要一個內部 IP 位址。

#### Virtual Machines

1. What are sustained use discounts?
    - [X] Automatic discounts that you get for running specific Compute Engine resources for a significant portion of the billing month
    - [ ] Purchase commitments for specific resources you know you will use
    - [ ] Per-second billing that starts after a 1 minute minimum 
    - [ ] Discounts you receive by using preemptible VM instances
> 說明：長期使用折扣：當資源在一個月內使用時間達標，會自動獲得折扣。

2. Which statement is true of Virtual Machine Instances in Compute Engine?
    - [X] In Compute Engine, a VM is a networked service that simulates the features of a computer.
    - [ ] All Compute Engine VMs are single tenancy and do not share CPU hardware. 
    - [ ] A VM in Compute Engine always maps to a single hardware computer in a rack.
    - [ ] Compute Engine uses VMware to create Virtual Machine Instances.
> 說明：Compute Engine 的虛擬機是模擬電腦功能的網路服務。

3. Which statement is true of persistent disks?
    - [ ] Persistent disks are always HDDs (magnetic spinning disks).
    - [X] Persistent disks are encrypted by default.
    - [ ] Persistent disks are physical hardware devices connected directly to VMs.
    - [ ] Once created, a persistent disk cannot be resized.
> 說明：持久磁碟預設會進行加密。

## Disclaimer
> [!CAUTION]
> The answers provided are for reference purposes only. If there are any errors, please kindly let us know.  
> Since the questions are part of the course's intellectual property, they may be used as a reference but must not be reproduced or distributed without permission to avoid violating intellectual property and copyright laws.