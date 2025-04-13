# MidTerm Exam Overview

- [MidTerm Exam Overview](#midterm-exam-overview)
  - [1. What is Cloud Computing?](#1-what-is-cloud-computing)
  - [2. Cloud Definitions](#2-cloud-definitions)
  - [3. Properties and Characteristics](#3-properties-and-characteristics)
  - [4. Central Ideas](#4-central-ideas)
  - [5. Scalability and Elasticity](#5-scalability-and-elasticity)
  - [6. Availability and Reliability](#6-availability-and-reliability)
  - [7. Manageability and Interoperability](#7-manageability-and-interoperability)
  - [8. Performance and Optimization](#8-performance-and-optimization)
  - [9. Accessibility and Portability](#9-accessibility-and-portability)
  - [10. Benefits from Cloud Computing](#10-benefits-from-cloud-computing)
  - [11. Service Models](#11-service-models)
  - [12. Deployment Models](#12-deployment-models)
  - [13. Summary](#13-summary)
  - [Disclaimer](#disclaimer)

## 1. What is Cloud Computing?
`Page: 4-9`

- **Definition**
  - Cloud computing is a model for enabling convenient, on-demand network access to a shared pool of configurable computing resources that can be rapidly provisioned and released with minimal management effort or service provider interaction. (NIST)
  - 中文說明：雲端運算是一種模型，能夠方便、隨需地存取共享且可配置的資源，並能以最少的管理努力迅速配置與釋放。

- **Different Perspectives**
  - Talk from Oracle CEO Larry Ellison  
    "We’ve redefined Cloud Computing to include everything that we already do. I don’t understand what we would do differently other than change the wording of some of our ads."
  - Talk from Rich Stallman  
    "It's stupidity. It's worse than stupidity: it's a marketing hype campaign. Somebody is saying this is inevitable – and whenever you hear somebody saying that, it's very likely to be a set of businesses campaigning to make it true."
  - 中文說明：Larry Ellison 認為雲端只是舊技術包裝；Richard Stallman 則認為雲端是行銷炒作。

---

## 2. Cloud Definitions
`Page: 5-9`

- **Wikipedia**
  - Cloud computing is Internet-based computing, whereby shared resources, software, and information are provided to computers and other devices on demand, like the electricity grid.
  - 中文說明：雲端運算是一種網際網路為基礎的運算模式，將資源、軟體與資訊按需提供，像電力一樣使用。

- **Whatis.com**
  - Cloud computing is a general term for anything that involves delivering hosted services over the Internet.
  - 中文說明：雲端運算泛指所有透過網路提供的託管服務。

- **Berkeley**
  - Cloud Computing refers to both the applications delivered as services over the Internet and the hardware and systems software in the datacenters that provide those services.
  - 中文說明：雲端包含了以服務形式交付的應用程式，以及背後支援的資料中心硬體與軟體。

- **Buyya**
  - A Cloud is a type of parallel and distributed system consisting of a collection of interconnected and virtualized computers that are dynamically provisioned and presented as one or more unified computing resources.
  - 中文說明：雲端是一種由互聯虛擬化計算機組成的平行與分散式系統，按需求整合為一個或多個統一資源。

---

## 3. Properties and Characteristics
`Page: 10`

- **Properties**
  - High scalability and elasticity
  - High availability and reliability
  - High manageability and interoperability
  - High accessibility and portability
  - High performance and optimization
  - 中文說明：雲端必須具備高擴展性、可用性、可靠性、管理性、互通性、存取性、可攜性與效能最佳化。

- **Enabling Techniques**
  - Hardware virtualization
  - Parallelized and distributed computing
  - Web service
  - 中文說明：雲端仰賴虛擬化、分散式計算及 Web 服務技術實現。

---

## 4. Central Ideas
`Page: 11-22`

- **Utility Computing**
  - Users do not want to own the physical infrastructure. Instead, they only want to pay as many as they used.
  - 中文說明：使用者只想按用量付費，不希望擁有硬體設備。

- **Service-Oriented Architecture (SOA)**
  - Service Oriented Architecture (SOA) is essentially a collection of services which communicate with each other.
  - 中文說明：SOA 是由多個可互相通訊的服務所構成的架構。

- **Service Level Agreement (SLA)**
  - A service-level agreement (SLA) is a contract between a network service provider and a customer that specifies, usually in measurable terms, what services the network service provider will furnish.
  - 中文說明：SLA 是服務提供者與客戶之間訂立的服務品質保證合約。

---

## 5. Scalability and Elasticity
`Page: 23-32`

- **Dynamic Provisioning**
  - Cloud resources should be provisioned dynamically to meet seasonal demand variations.
  - 中文說明：雲端資源應根據需求動態調整。

- **Multi-tenant Design**
  - Multi-tenant refers to a principle in software architecture where a single instance of the software runs on a server, serving multiple client organizations.
  - 中文說明：一個系統同時服務多個客戶，每個客戶像是獨立環境。

---

## 6. Availability and Reliability
`Page: 33-41`

- **Fault Tolerance**
  - Fault-tolerance is the property that enables a system to continue operating properly in the event of the failure of some of its components.
  - 中文說明：容錯性是指部分故障時系統仍能正常運作。

- **System Resilience**
  - Resilience is the ability to provide and maintain an acceptable level of service in the face of faults and challenges to normal operation.
  - 中文說明：韌性是系統在異常時仍可快速恢復服務的能力。

- **System Security**
  - Cloud security is an evolving sub-domain of computer security, network security, and information security.
  - 中文說明：雲端安全屬於資訊安全領域的延伸分支。

---

## 7. Manageability and Interoperability
`Page: 42-50`

- **Autonomic Computing**
  - Its ultimate aim is to develop computer systems capable of self-management.
  - 中文說明：自主運算系統可自我管理與修復。

- **System Monitoring**
  - A System Monitor is a process within a distributed system for collecting and storing state data.
  - 中文說明：系統監控負責監測系統狀態。

- **Billing System**
  - Users pay as many as they used.
  - 中文說明：使用者依使用量付費。

---

## 8. Performance and Optimization
`Page: 51-57`

- **Parallel Processing**
  - Parallel processing is a form of computation in which many calculations are carried out simultaneously.
  - 中文說明：平行運算是同時處理多筆計算。

- **Load Balancing**
  - Load balancing is a technique to distribute workload evenly across two or more computers.
  - 中文說明：負載平衡將系統負載平均分散。

- **Job Scheduling**
  - A job scheduler is a software application that is in charge of unattended background executions.
  - 中文說明：作業排程軟體負責自動執行背景作業。

---

## 9. Accessibility and Portability
`Page: 58-61`

- **Uniform Access**
  - Users from different operating systems or other accessing platforms should be able to directly be served.
  - 中文說明：不同平台的使用者應能無縫存取服務。

- **Thin Client**
  - Thin client is a computer or a computer program which depends heavily on some other computer to fulfill its traditional computational roles.
  - 中文說明：瘦客戶端依賴伺服器完成運算。

---

## 10. Benefits from Cloud Computing
`Page: 62-78`

- **Reduce Initial Investment**
  - Enterprise do not need to own the infrastructure.
  - 中文說明：企業無需擁有硬體設備，降低初期投資。

- **Reduce Capital Expenditure**
  - Enterprise can almost dismiss its IT department.
  - 中文說明：企業可縮減 IT 部門規模，降低支出。

- **Improve Industrial Specialization**
  - Industrial specialization will be improved.
  - 中文說明：專業分工將因雲端服務而提升。

- **Improve Resource Utilization**
  - IT infrastructure performance and utilization can be optimized.
  - 中文說明：IT 資源將因集中管理而最佳化。

- **Reduce Local Computing Power**
  - Only basic hardware to connect to internet.
  - 中文說明：只需簡單裝置即可連網使用強大運算力。

- **Reduce Local Storage Power**
  - Dynamically allocated on demand.
  - 中文說明：儲存空間可依需求動態調整。

- **Variety of Thin Client Devices**
  - Accessed through small smart devices.
  - 中文說明：可使用小型智慧裝置存取雲端。

---

## 11. Service Models
`Page: 79-108`

- **IaaS**
  - Infrastructure as a Service - Provision processing, storage, networks, and other fundamental computing resources.
  - 中文說明：IaaS 提供虛擬化硬體資源。

- **PaaS**
  - Platform as a Service - Deploy onto the cloud infrastructure consumer-created or acquired applications.
  - 中文說明：PaaS 提供應用開發與部署平台。

- **SaaS**
  - Software as a Service - Use the provider’s applications running on a cloud infrastructure.
  - 中文說明：SaaS 直接租用雲端應用程式。

---

## 12. Deployment Models
`Page: 111-117`

- **Public Cloud**
  - The cloud infrastructure is made available to the general public or a large industry group.
  - 中文說明：公有雲開放給一般大眾或產業集團使用。

- **Private Cloud**
  - The cloud infrastructure is operated solely for an organization.
  - 中文說明：私有雲僅限單一組織專用。

- **Community Cloud**
  - The cloud infrastructure is shared by several organizations and supports a specific community.
  - 中文說明：社群雲由多個組織共同使用。

- **Hybrid Cloud**
  - The cloud infrastructure is a composition of two or more clouds.
  - 中文說明：混合雲結合了多種雲端架構。

---

## 13. Summary
`Page: 118-120`

- **Summary**
  - Cloud computing is a new paradigm shift of computing that provides high quality properties and services.
  - 中文說明：雲端運算是一種新的運算典範，帶來高品質的特性與服務。

---

## Disclaimer
> [!CAUTION]
> The answers provided are for reference purposes only. If there are any errors, please kindly let us know.  
> Since the questions are part of the course's intellectual property, they may be used as a reference but must not be reproduced or distributed without permission to avoid violating intellectual property and copyright laws.