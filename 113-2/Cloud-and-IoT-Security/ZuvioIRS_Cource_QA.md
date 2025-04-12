# Cloud and IoT Security Cource Q&A (Zuvio IRS)
- [Cloud and IoT Security Cource Q\&A (Zuvio IRS)](#cloud-and-iot-security-cource-qa-zuvio-irs)
  - [MidTerm Exam](#midterm-exam)
    - [Week 2 - Introduction to Cloud Computing](#week-2---introduction-to-cloud-computing)
    - [Week 5 - Virtual Machine and Networks in the Cloud](#week-5---virtual-machine-and-networks-in-the-cloud)
    - [Week 5 - Sotrage in the Cloud](#week-5---sotrage-in-the-cloud)
    - [Week 6](#week-6)
    - [Week 7](#week-7)
    - [Week 8](#week-8)
  - [Final Project (Comming Soon...)](#final-project-comming-soon)
  - [Disclaimer](#disclaimer)

## MidTerm Exam

### Week 2 - Introduction to Cloud Computing

1.__________ is a desirable property of a system, a network, or a process, which indicates its ability to either handle growing amounts of work in a graceful manner or to be readily enlarged.
> **Answers:** Scalability

> **說明：** 指系統或網路能夠優雅地應對工作量增加，或容易擴充的特性。

2.What is dynamic provisioning in cloud computing?
> **Answers:**  Dynamic provisioning automatically allocates or deallocates computing resources based on real-time demand.

> **說明：** 動態資源配置會根據即時需求，自動分配或回收運算資源。

3. What is system monitoring in cloud computing?
> **Answers:** System monitoring tracks the performance, availability, and health of cloud resources.

> **說明：** 系統監控用來追蹤雲端資源的效能、可用性與健康狀況。

4. What is portability and interoperability issues in cloud computing?
> **Answers:** Portability means moving applications across environments easily; interoperability means different systems can work together.

> **說明：** 可攜性指應用程式能在不同環境間移動；互通性指不同系統間能相互運作。

5. The ultimate goal of ___________ is to develop computer systems capable of self-management, to overcome the rapidly growing complexity of computing systems management, and to reduce the barrier that complexity poses to further growth.
> **Answers:** Autonomic Computing

> **說明：** 自主運算旨在開發能自我管理的電腦系統，以應對管理複雜性，促進未來成長。

6. What is a Job Scheduler?
> **Answers:** A Job Scheduler automatically manages and schedules tasks or jobs to run at specific times or under certain conditions.

> **說明：** 作業排程器會自動管理並安排任務在特定時間或條件下執行。

7.  What kind of services should be scheduled? Please give 2 examples.
> **Answers:** Examples: Backup tasks, system updates.

> **說明：** 例如備份任務、系統更新。

8.  Why is load balancing important?
> **Answers:** Load balancing distributes traffic evenly across servers to improve reliability and performance.

> **說明：** 負載平衡將流量平均分配到伺服器上，以提升可靠性和效能。

### Week 5 - Virtual Machine and Networks in the Cloud

1.What is Google Cloud VPC?
> **Answers:** Google Cloud VPC is a virtual private network that provides secure and scalable communication between cloud resources.

> **說明：** Google Cloud VPC 是一個虛擬私人網路，提供雲端資源之間安全且可擴展的通訊環境。

2.What can be achieved with Google Cloud VPC?
> **Answers:** You can manage IP address ranges, subnets, firewalls, routes, and VPN connections for secure communication.

> **說明：** 可以管理 IP 範圍、子網路、防火牆、路由及 VPN 連線，確保安全通訊。

3. Creating a virtual machine in GCP, please name at least 4 configurations you must setup.
> **Answers:** Machine type, disk type and size, network settings, region/zone.

> **說明：** 建立虛擬機時必須設定：機器類型、磁碟類型與大小、網路設定、區域/可用區域。

4. "A one-month prepayment have to be made for using VMs in GCP." Is the statement correct? If isn't, please provide your explanation.
> **Answers:** False. GCP VMs are charged per second of usage, not prepaid monthly.

> **說明：** 錯誤，GCP 的虛擬機按秒計費，不需事先預付一個月費用。

5. Why are developers use VMs instead of spinning up their own infrastructure?
> **Answers:** VMs are faster to deploy, scalable, cost-effective, and managed by cloud providers.

> **說明：** 虛擬機部署快速、易於擴展、成本效益高，由雲端供應商管理。

6. Compute Engine has a feature called __________, where VMs can be added to or subtracted from an application based on load metrics.

> **Answers:** Autoscaling
> **說明：** 自動擴展可根據負載自動增加或減少虛擬機。

1. Why setting up VMs with NO auto-scaling may affect service availability when the traffic spikes?
> **Answers:** Without auto-scaling, the fixed number of VMs cannot handle traffic spikes, causing slowdowns or failures.

> **說明：** 若未啟用自動擴展，固定數量的虛擬機無法應付流量高峰，容易導致服務變慢或中斷。

8. What are the core components of a Google Cloud VPC network? Please name a few and explain their functions respectively.
> **Answers:** 
> - Subnetworks: Divide the IP address range into smaller segments.
> - Routes: Control how traffic flows within the network.
> - Firewall Rules: Define rules to allow or deny network traffic.
> - VPN Gateways: Connect on-premises data centers to Google Cloud securely.

> **說明：** VPC 核心包含子網路、路由、防火牆規則與 VPN 閘道，各自負責 IP 劃分、流量控制、安全管理及跨地點連線。
> - Subnetworks：劃分 IP 範圍。
> - Routes：控制資料流向。
> - Firewall Rules：設定流量允許或拒絕規則。
> - VPN Gateways：連接本地資料中心與雲端。

1. _________ load balancers operate at the application layer and are designed to handle HTTP and HTTPS traffic, whereas _________ load balancers operate at the transport layer and efficiently handle TCP, UDP, and other IP protocols.
> **Answers:** HTTP(S) Load Balancers；TCP/UDP Load Balancers

> **說明：** HTTP(S) 負載平衡器運作在應用層；TCP/UDP 負載平衡器運作在傳輸層。

10. Without a load balancer, cloud resources often become inefficiently utilized, Why?
> **Answers:** Because some servers may be overloaded while others are idle, causing resource waste and poor performance.

> **說明：** 部分伺服器超載，部分閒置，導致資源浪費和效能下降。

11. DNS is what translates ___________ to ___________, and as you might imagine, Google has a highly developed DNS infrastructure.
> **Answers:** Domain names；IP addresses

> **說明：** DNS 將網域名稱轉換為 IP 位址。

12. What's the benefit of using Cloud CDN?
> **Answers:** It reduces latency and offloads traffic from the origin server by caching content closer to users.

> **說明：** Cloud CDN 透過將內容快取在接近用戶的位置，減少延遲並降低原始伺服器負載。

13. Without CDN, the system load and traffic consumption of the origin server may increase dramatically. Why?
> **Answers:** Because every user request has to be processed by the origin server directly, increasing its load.

> **說明：** 因所有用戶請求都要回到原始伺服器處理，造成負載劇增。

14. What's the role of Cloud Router in Google Cloud VPC Network?
> **Answers:** Cloud Router dynamically exchanges routes between your VPC and on-premises networks using BGP.

> **說明：** Cloud Router 透過 BGP 協定，動態交換 VPC 與本地網路之間的路由資訊。

15. Please explain the upside and downside of using:
    a) Peering 
    b) Dedicated Interconnect to connect to Google Cloud VPC.
> **Answers:** </br>
> a) Peering
> - Upside: Simple and low cost.
> - Downside: Limited traffic and no guaranteed bandwidth.

> b) Dedicated Interconnect
> - Upside: High bandwidth, low latency, and a dedicated connection.
> - Downside: High cost and requires more configuration.

> **說明：**</br>
> a) Peering
> - 優點：簡單，低成本。
> - 缺點：流量受限，無保證帶寬。

> b) Dedicated Interconnect
> - 優點：高頻寬、低延遲、專屬線路。
> - 缺點：成本高，需要更多設定。

### Week 5 - Sotrage in the Cloud
1. Google Drive (What you use daily as a consumer) and GCP Cloud Storage all stores data. What makes them different?
> **Answers:** Google Drive is a consumer-focused cloud storage service for personal file sharing and synchronization, while GCP Cloud Storage is an enterprise-grade storage service offering high scalability, durability, and availability for application development and deployment.

> **說明︰** Google Drive 是面向消費者的雲端儲存，用於個人文件分享與同步；GCP Cloud Storage 則是企業級儲存服務，提供高擴展性、耐用性與可用性，適合應用開發與部署。

2. What does the term "Object Storage" mean?
> **Answers:** Object Storage is a storage architecture that manages data as objects, where each object includes the data itself, metadata, and a unique identifier.

> **說明：** 物件儲存是一種將資料以「物件」形式管理的架構，每個物件包含資料本身、元資料與唯一識別碼。

3. GCP Cloud Storage has many storage classes. Could you give brief explanations on them respectively?
> **Answers:**
> - Standard: For frequently accessed data.
> - Nearline: For data accessed about once a month.
> - Coldline: For data accessed a few times a year.
> - Archive: For long-term storage with rare access.

> **說明：**
> - Standard：適合經常存取的資料。
> - Nearline：適合大約一個月存取一次的資料。
> - Coldline：適合一年存取幾次的資料。
> - Archive：適合長期儲存且極少存取的資料。
1. Why it is "cost effective" to choose a proper storage class to store data?
> **Answers:** Choosing the proper storage class based on access frequency can minimize storage costs while meeting performance needs.

> **說明：** 根據資料存取頻率選擇適合的儲存等級，可以在滿足效能需求的同時，降低儲存成本。

1. Why would developers choose GCP Cloud SQL over self-maintained SQL database infrastructure?
> **Answers:** GCP Cloud SQL offers managed services including automated backups, updates, and high availability without the need for manual maintenance.

> **說明：** GCP Cloud SQL 提供自動備份、更新與高可用性的管理式服務，開發者無需自行維護資料庫。

1. Cloud Storage, SQL, Spanner, Bigtable and Firestore are all data storage solutions provided by GCP.
    Please provide the most suitable storage option to the following use cases.
    a) _______ is best for storing relational data with MySQL, Postgres, or SQL Server.
    b) _______ is best for storing binary data such as images, videos and documents.
    c) _______ is globally distributed, stores relational data, and is best for horizontal scaling, high availability, throughput and consistency.
    d) _______ is best for storing analytical data and heavy read / write.
    e) _______ is best for real time query needs.
> **Answers:**
> a) Cloud SQL
> b) Cloud Storage
> c) Cloud Spanner
> d) Bigtable
> e) Firestore

> **說明：**
> a) Cloud SQL：適合儲存 MySQL、Postgres 或 SQL Server 的關聯式資料。
> b) Cloud Storage：適合儲存圖片、影片與文件等二進位資料。
> c) Cloud Spanner：適合全球分佈、需要水平擴展與高可用性的關聯式資料。
> d) Bigtable：適合儲存分析資料與高讀寫負載資料。
> e) Firestore：適合即時查詢需求。

### Week 6

1. Explain the differences between
    a) Containers
    b) Virtual Machines
    c) Your own computer / laptop
> **Answers:**
> a) Containers: Lightweight, share the host OS kernel, and package applications with dependencies.

> b) Virtual Machines: Full OS virtualization, each VM has its own OS and hardware emulation.

> c) Computer/Laptop: Physical hardware running a full operating system directly.

> **說明：**
> - 容器：輕量化，共享主機的作業系統核心，打包應用程式與其依賴。
> - 虛擬機：完整虛擬化，每個虛擬機都有自己的作業系統與硬體模擬。
> - 電腦/筆電：直接運行完整作業系統的實體硬體。

2. Why might developers choose containers over virtual machines (VMs)?
> **Answers:** Containers are more lightweight, faster to start, consume fewer resources, and are easier to scale.

**說明：** 容器更輕量、啟動速度快、資源消耗少，且更容易擴展。
3. What does "container orchestration" mean?
> **Answers:** Container orchestration is the automated management, deployment, scaling, and networking of containers.

> **說明：** 容器編排是指自動化地管理、部署、擴展及網路連接容器。

4. What the so-called "Pods" in Kubernetes?
> **Answers:** A Pod is the smallest deployable unit in Kubernetes, consisting of one or more containers that share storage and network resources.

> **說明：** Pod 是 Kubernetes 中最小的可部署單位，包含一個或多個共享儲存與網路資源的容器。

5. Why might developers choose Kubernetes instead of managing containers on their own?
> **Answers:** Kubernetes automates container deployment, scaling, and operations, reducing manual workload and increasing system reliability.

> **說明：** Kubernetes 自動化容器的部署、擴展與操作，減少人工負擔並提高系統可靠性。

6. Provide a list of the automation capabilities that Google Kubernetes Engine (GKE) offers to developers.
> **Answers:** Auto-scaling, auto-healing, automatic updates, logging, monitoring, and load balancing.

> **說明：** GKE 提供自動擴展、自我修復、自動更新、日誌紀錄、監控與負載平衡等功能。

7. What does the development workflow looks like using GCP Cloud Run?
> **Answers:** Developers build a container, push it to a container registry, and deploy it directly to Cloud Run where it automatically scales based on traffic.

> **說明：** 開發者建立容器、推送至容器註冊庫，然後直接部署到 Cloud Run，服務會根據流量自動擴展。

8. GCP Cloud Run is "Stateless". What does "Stateless" mean?
> **Answers:** Stateless means each request is independent, and the server does not retain information about past requests.

> **說明：** 無狀態意味著每個請求都是獨立的，伺服器不會記錄過去的請求資訊。

9.  GCP Cloud Run is a "Serverless Service". What does "Serverless" mean?
> **Answers:** Serverless means developers don't manage servers; they only focus on deploying code while the platform handles infrastructure automatically.

> **說明：** 無伺服器（Serverless）指的是開發者無需管理伺服器，只需專注於部署程式碼，由平台自動管理基礎設施。

10. Please point out at least 2 advantages and 2 disadvantages of serverless services.
> **Answers:**
> - Advantages: No server management, automatic scaling.
> - Disadvantages: Cold start latency, limited control over infrastructure.

> **說明：**
> - 優點：無需管理伺服器、自動擴展。
> - 缺點：冷啟動延遲、對基礎設施控制有限。

11. Why does serverless computing often cost more than VMs for consistent workloads, but become more economical for lightweight or intermittent usage patterns?
> **Answers:** Serverless charges based on usage time and invocations, making it expensive for continuous workloads but cheaper for sporadic or low-traffic applications.

> **說明：** Serverless 根據使用時間與次數計費，對於持續性工作負載來說較昂貴，但對於零星或低流量應用則更經濟。

12. **(Continued)** Based on the cost considerations in the previous question, which specific use cases would you consider most suitable for serverless services such as Cloud Run? Please justify your answers.
> **Answers:** Best suited for event-driven applications, APIs, lightweight microservices, or applications with unpredictable traffic patterns.

> **說明：** 最適合事件驅動應用程式、API、輕量微服務，或流量不可預測的應用場景。

### Week 7

1. What are the 4 ways to access GCP? And how?
> **Answers:**
> - Cloud Console: Web-based GUI.
> - Cloud Shell: In-browser command-line interface.
> - Cloud SDK (gcloud CLI): Local command-line tool.
> - Client Libraries: Programmatic access via supported programming languages.

> **說明：** </br>
> - Cloud Console：網頁圖形介面。
> - Cloud Shell：瀏覽器內的命令列介面。
> - Cloud SDK（gcloud CLI）：本機指令列工具。
> - Client Libraries：透過程式語言以程式方式存取。
2. Take a look at the GCP Cloud Shell documentation webpage and answer question 1:
   - 1) What are at least 3 things you can do with Cloud Shell?
   - 2) Based on the Lab contents, what is the Cloud Shell basically? (What is running the Cloud Shell?) And if you want to create a persistent state in Cloud Shell, what should you do?
> **Answers:** </br>
> 1)
> - Manage GCP resources with command-line tools.
> - Edit files using an integrated code editor.
> - Run Cloud SDK and other pre-installed tools.</br>
> 2)
> - Cloud Shell is a Debian-based virtual machine provisioned temporarily. To create a persistent state, you should store files in the `$HOME` directory, which persists across sessions.

> **說明：** </br>
> 1)
> - 使用命令列工具管理 GCP 資源。
> - 使用內建的程式碼編輯器編輯檔案。
> - 執行 Cloud SDK 與其他預先安裝的工具。 </br>
> 2) Cloud Shell 是基於 Debian 的虛擬機，為暫時性資源。若要保持資料，需將檔案儲存於 `$HOME` 目錄中，該目錄會在不同工作階段間保留。



3. (True / False) Cloud Marketplace lets you easily set up services with software that comes packaged with predefined machine types and networks for one-click installation.
> **Answers:** True.

> **說明：** 正確，Cloud Marketplace 可以讓你一鍵安裝包含預設機型與網路配置的軟體服務。

4. (True / False) After launching a Marketplace package, you can still change the settings if you want.
> **Answers:** True.

> **說明：** 正確，啟動 Marketplace 套件後，仍然可以更改設定。

5. What's the difference between an internal IP address and an external IP address?
> **Answers:** Internal IP addresses are used within a private network, while external IP addresses are accessible from the internet.

> **說明：** 內部 IP 位址用於私人網路中，外部 IP 位址則可從互聯網上存取。

6. Why would developers use internal IP addresses for communication between VMs in the same GCP region, instead of external IP addresses, when both options can achieve the same goal?
> **Answers:** Internal IP addresses reduce cost and increase security compared to external IP addresses.

> **說明：** 使用內部 IP 位址可以降低成本並提升安全性，相比外部 IP 位址更具優勢。

7. (True / False) Every VM created in GCP will be automatically binded with an external IP address.
> **Answers:** False.

> **說明：** 錯誤，並非所有建立的虛擬機都會自動綁定外部 IP 位址。

### Week 8

1. Please name a few common network design techniques you can use in Google Cloud.
> **Answers:**
> - VPC Peering
> - VPN Gateway
> - Cloud Interconnect
> - Load Balancing
> - Private Google Access

> **說明：**
> - VPC 對等連線
> - VPN 閘道
> - 雲端專線互連
> - 負載平衡
> - 私有 Google 存取

2. Please briefly describe VM Lifecycles.
> **Answers:** VM lifecycles include creation, running, stopping, suspending, resuming, and terminating instances.

> **說明：** 虛擬機生命周期包含建立、運行、停止、暫停、恢復與終止等狀態。

3. (True or False) "A terminated VM instance will not incur charges."
> **Answers:** False.

> **說明：** 錯誤，即使虛擬機進入`Terminated`狀態，user還是要支付`Disk`跟`IP address的費用，除非虛擬機完全被刪除。

4. Why are VM instances divided into many different categories? What's the purpose of that?
> **Answers:** VM instances are categorized to optimize cost and performance for different workloads, such as general-purpose, memory-optimized, and compute-optimized tasks.

> **說明：** 將虛擬機分成不同類型，能針對不同工作負載（如通用型、記憶體優化型、運算優化型）最佳化成本與效能。

5. John Cenee is looking for a cost-effective VM solution for his large-scale, computation-intensive tasks. Which VM type would you recommend? Please justify your answer.
> **Answers:** Recommend `Compute-Optimized VMs (C2 instances)` because they offer high CPU performance suitable for heavy computation tasks.

> **說明：** 建議使用`運算優化型虛擬機（C2 類型）`，因為它們提供高 CPU 效能，適合大量計算任務。

6. Trumpee is looking for an economical VM solution to calculate basic math for his tariffs. While we suggest doing the calculations on his own, he insists on using VMs. His workload fluctuates significantly, with sudden spikes in demand followed by long periods of inactivity. What kind of VM type and configuration would you recommand? Please justify your answer.
> **Answers:** Recommend `Preemptible VMs` or `Auto-scaling managed instance groups` to handle fluctuating workloads cost-effectively.

> **說明：** 建議使用`可搶占式虛擬機`或`自動擴展的受控實例群組`，以應對波動性大的工作負載並控制成本。

7. Kamalee is looking for a VM solution for her predictable, consistent workload with an expected 2-year usage period. She's on a tight budget. Is there any way to get a discounted price? Please explain your answer.
> **Answers:** Yes, using `Committed Use Discounts` can significantly reduce costs by committing to a one- or three-year usage plan.

> **說明：** 可以，使用 承諾使用折扣（Committed Use Discounts），承諾使用一至三年，可以大幅降低成本。

## Final Project (Comming Soon...)
> [!NOTE]
> Stay tuned for updates.


## Disclaimer
> [!CAUTION]
> The answers provided are for reference purposes only. If there are any errors, please kindly let us know.  
> Since the questions are part of the course's intellectual property, they may be used as a reference but must not be reproduced or distributed without permission to avoid violating intellectual property and copyright laws.