# Cloud and IoT Security Cource (Zuvio IRS - Q&A)

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
> **說明：**

4. What is portability and interoperability issues in cloud computing?
> **Answers:**

> **說明：**

5. The ultimate goal of ___________ is to develop computer systems capable of self-management, to overcome the rapidly growing complexity of computing systems management, and to reduce the barrier that complexity poses to further growth.
> **Answers:**
> 
> **說明：**

6. What is a Job Scheduler?
> **Answers:**
>
> **說明：**

7.  What kind of services should be scheduled? Please give 2 examples.
> **Answers:**
> **說明：**

8.  Why is load balancing important?
> **Answers:**
> **說明：**
> 
9.  What is a Job Scheduler?
> **Answers:**
> **說明：**

10.  What kind of services should be scheduled? Please give 2 examples.
> **Answers:**
> **說明：**

11.  Why is load balancing important?
> **Answers:**
> **說明：**

### Week 5 - Virtual Machine and Networks in the Cloud

1.What is Google Cloud VPC?
> **Answers:**
> **說明：**

2.What can be achieved with Google Cloud VPC?
> **Answers:**
> **說明：**

3. Creating a virtual machine in GCP, please name at least 4 configurations you must setup.
> **Answers:**
> **說明：**

4. "A one-month prepayment have to be made for using VMs in GCP." Is the statement correct? If isn't, please provide your explanation.
> **Answers:**
> **說明：**

5. Why are developers use VMs instead of spinning up their own infrastructure?
> **Answers:**
> **說明：**

6. Compute Engine has a feature called __________, where VMs can be added to or subtracted from an application based on load metrics.
> **Answers:**
> **說明：**

7. Why setting up VMs with NO auto-scaling may affect service availability when the traffic spikes?
> **Answers:**
> **說明：**

8. What are the core components of a Google Cloud VPC network? Please name a few and explain their functions respectively.
> **Answers:**
> **說明：**

9. _________ load balancers operate at the application layer and are designed to handle HTTP and HTTPS traffic, whereas _________ load balancers operate at the transport layer and efficiently handle TCP, UDP, and other IP protocols.
> **Answers:**
> **說明：**

10. Without a load balancer, cloud resources often become inefficiently utilized, Why?
> **Answers:**
> **說明：**

11. DNS is what translates ___________ to ___________, and as you might imagine, Google has a highly developed DNS infrastructure.
> **Answers:**
> **說明：**

12. What's the benefit of using Cloud CDN?
> **Answers:**
> **說明：**

13. Without CDN, the system load and traffic consumption of the origin server may increase dramatically. Why?
> **Answers:**
> **說明：**

14. What's the role of Cloud Router in Google Cloud VPC Network?
> **Answers:**
> **說明：**

15. Please explain the upside and downside of using:
    a) Peering 
    b) Dedicated Interconnect to connect to Google Cloud VPC.
> **Answers:**
> a) 
> b)
> **說明：**

### Week 5 - Sotrage in the Cloud
1. Google Drive (What you use daily as a consumer) and GCP Cloud Storage all stores data. What makes them different?
> **Answers:**
> **說明︰**

2. What does the term "Object Storage" mean?
> **Answers:**
> **說明：**

3. GCP Cloud Storage has many storage classes. Could you give brief explanations on them respectively?
> **Answers:**
> **說明：**

4. Why it is "cost effective" to choose a proper storage class to store data?
> **Answers:**
> **說明：**

5. Why would developers choose GCP Cloud SQL over self-maintained SQL database infrastructure?
> **Answers:**
> **說明：**

6. Cloud Storage, SQL, Spanner, Bigtable and Firestore are all data storage solutions provided by GCP.
    Please provide the most suitable storage option to the following use cases.
    a) _______ is best for storing relational data with MySQL, Postgres, or SQL Server.
    b) _______ is best for storing binary data such as images, videos and documents.
    c) _______ is globally distributed, stores relational data, and is best for horizontal scaling, high availability, throughput and consistency.
    d) _______ is best for storing analytical data and heavy read / write.
    e) _______ is best for real time query needs.
> **Answers:**
> a)
> b)
> c)
> d)
> e)
> **說明：**

### Week 6

1. Explain the differences between
    a) Containers
    b) Virtual Machines
    c) Your own computer / laptop
> **Answers:**
> a)
> b)
> c)
> **說明：**

2. Why might developers choose containers over virtual machines (VMs)?
> **Answers:**
**說明：**
3. What does "container orchestration" mean?
> **Answers:**
> **說明：**

4. What the so-called "Pods" in Kubernetes?
> **Answers:**
> **說明：**

5. Why might developers choose Kubernetes instead of managing containers on their own?
> **Answers:**
> **說明：**

6. Provide a list of the automation capabilities that Google Kubernetes Engine (GKE) offers to developers.
> **Answers:**
> **說明：**

7. What does the development workflow looks like using GCP Cloud Run?
> **Answers:**
> **說明：**

8. GCP Cloud Run is "Stateless". What does "Stateless" mean?
> **Answers:**
> **說明：**

9.  GCP Cloud Run is a "Serverless Service". What does "Serverless" mean?
> **Answers:**
> **說明：**

10. Please point out at least 2 advantages and 2 disadvantages of serverless services.
> **Answers:**
> **說明：**

11. Why does serverless computing often cost more than VMs for consistent workloads, but become more economical for lightweight or intermittent usage patterns?
> **Answers:**
> **說明：**

12. **(Continued)** Based on the cost considerations in the previous question, which specific use cases would you consider most suitable for serverless services such as Cloud Run? Please justify your answers.
> **Answers:**
> **說明：**

### Week 7

1. What are the 4 ways to access GCP? And how?
> **Answers:**
> **說明：**

2. Take a look at the GCP Cloud Shell documentation webpage and answer question 1:

   1) What are at least 3 things you can do with Cloud Shell?

   2) Based on the Lab contents, what is the Cloud Shell basically? (What is running the Cloud Shell?) And if you want to create a persistent state in Cloud Shell, what should you do?
> **Answers:**
> 1)
> 2)
> **說明：**

3. (True / False) Cloud Marketplace lets you easily set up services with software that comes packaged with predefined machine types and networks for one-click installation.
> **Answers:**
> **說明：**

4. (True / False) After launching a Marketplace package, you can still change the settings if you want.
> **Answers:**
> **說明：**

5. What's the difference between an internal IP address and an external IP address?
> **Answers:**
> **說明：**

6. Why would developers use internal IP addresses for communication between VMs in the same GCP region, instead of external IP addresses, when both options can achieve the same goal?
> **Answers:**
> **說明：**

7. (True / False) Every VM created in GCP will be automatically binded with an external IP address.
> **Answers:**
> **說明：**

### Week 8

1. Please name a few common network design techniques you can use in Google Cloud.
> **Answers:**
> **說明：**

2. Please briefly describe VM Lifecycles.
> **Answers:**
> **說明：**

3. (True or False) "A terminated VM instance will not incur charges."
> **Answers:**
> **說明：**

4. Why are VM instances divided into many different categories? What's the purpose of that?
> **Answers:**
> **說明：**

5. John Cenee is looking for a cost-effective VM solution for his large-scale, computation-intensive tasks. Which VM type would you recommend? Please justify your answer.
> **Answers:**
> **說明：**

6. Trumpee is looking for an economical VM solution to calculate basic math for his tariffs. While we suggest doing the calculations on his own, he insists on using VMs. His workload fluctuates significantly, with sudden spikes in demand followed by long periods of inactivity. What kind of VM type and configuration would you recommand? Please justify your answer.
> **Answers:**
> **說明：**

7. Kamalee is looking for a VM solution for her predictable, consistent workload with an expected 2-year usage period. She's on a tight budget. Is there any way to get a discounted price? Please explain your answer.
> **Answers:**
> **說明：**