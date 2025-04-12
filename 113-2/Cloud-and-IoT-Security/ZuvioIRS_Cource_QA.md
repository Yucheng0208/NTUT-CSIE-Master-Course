# Cloud and IoT Security Cource (Zuvio IRS - Q&A)

## MidTerm Exam

### Week 2 - Introduction to Cloud Computing

1.__________ is a desirable property of a system, a network, or a process, which indicates its ability to either handle growing amounts of work in a graceful manner or to be readily enlarged.

2.What is dynamic provisioning in cloud computing?

3.__________ is a desirable property of a system, a network, or a process, which indicates its ability to either handle growing amounts of work in a graceful manner or to be readily enlarged.

4.What is dynamic provisioning in cloud computing?

5. What is system monitoring in cloud computing?

6. What is portability and interoperability issues in cloud computing?

7. The ultimate goal of ___________ is to develop computer systems capable of self-management, to overcome the rapidly growing complexity of computing systems management, and to reduce the barrier that complexity poses to further growth.

8. What is a Job Scheduler?

9.  What kind of services should be scheduled? Please give 2 examples.

10. Why is load balancing important?

11. What is a Job Scheduler?

12. What kind of services should be scheduled? Please give 2 examples.

13. Why is load balancing important?

### Week 5 - Virtual Machine and Networks in the Cloud

1.What is Google Cloud VPC?

2.What can be achieved with Google Cloud VPC?

3. Creating a virtual machine in GCP, please name at least 4 configurations you must setup.

4. "A one-month prepayment have to be made for using VMs in GCP." Is the statement correct? If isn't, please provide your explanation.

5. Why are developers use VMs instead of spinning up their own infrastructure?

6. Compute Engine has a feature called __________, where VMs can be added to or subtracted from an application based on load metrics.

7. Why setting up VMs with NO auto-scaling may affect service availability when the traffic spikes?

8. What are the core components of a Google Cloud VPC network? Please name a few and explain their functions respectively.

9. _________ load balancers operate at the application layer and are designed to handle HTTP and HTTPS traffic, whereas _________ load balancers operate at the transport layer and efficiently handle TCP, UDP, and other IP protocols.

10. Without a load balancer, cloud resources often become inefficiently utilized. Why?

11. DNS is what translates ___________ to ___________, and as you might imagine, Google has a highly developed DNS infrastructure.

12. What's the benefit of using Cloud CDN?

13. Without CDN, the system load and traffic consumption of the origin server may increase dramatically. Why?
14. What's the role of Cloud Router in Google Cloud VPC Network?

15. Please explain the upside and downside of using
    a) Peering, and
    b) Dedicated Interconnect to connect to Google Cloud VPC.

### Week 5 - Sotrage in the Cloud
1. Google Drive (What you use daily as a consumer) and GCP Cloud Storage all stores data. What makes them different?

2. What does the term "Object Storage" mean?

3. GCP Cloud Storage has many storage classes. Could you give brief explanations on them respectively?

4. Why it is "cost effective" to choose a proper storage class to store data?

5. Why would developers choose GCP Cloud SQL over self-maintained SQL database infrastructure?

6. Cloud Storage, SQL, Spanner, Bigtable and Firestore are all data storage solutions provided by GCP.
    Please provide the most suitable storage option to the following use cases.
    a) _______ is best for storing relational data with MySQL, Postgres, or SQL Server.
    b) _______ is best for storing binary data such as images, videos and documents.
    c) _______ is globally distributed, stores relational data, and is best for horizontal scaling, high availability, throughput and consistency.
    d) _______ is best for storing analytical data and heavy read / write.
    e) _______ is best for real time query needs.

### Week 6

1. Explain the differences between
    a) Containers
    b) Virtual Machines
    c) Your own computer / laptop

2. Why might developers choose containers over virtual machines (VMs)?

3. What does "container orchestration" mean?

4. What the so-called "Pods" in Kubernetes?

5. Why might developers choose Kubernetes instead of managing containers on their own?

6. Provide a list of the automation capabilities that Google Kubernetes Engine (GKE) offers to developers.

7. What does the development workflow looks like using GCP Cloud Run?

8. GCP Cloud Run is "Stateless". What does "Stateless" mean?

9.  GCP Cloud Run is a "Serverless Service". What does "Serverless" mean?

10. Please point out at least 2 advantages and 2 disadvantages of serverless services.

11. Why does serverless computing often cost more than VMs for consistent workloads, but become more economical for lightweight or intermittent usage patterns?

12. (Q11 Continued) Based on the cost considerations in the previous question, which specific use cases would you consider most suitable for serverless services such as Cloud Run? Please justify your answers.

### Week 7

1. What are the 4 ways to access GCP? And how?

2. Take a look at the GCP Cloud Shell documentation webpage and answer question 1:

   1) What are at least 3 things you can do with Cloud Shell?

   2) Based on the Lab contents, what is the Cloud Shell basically? (What is running the Cloud Shell?) And if you want to create a persistent state in Cloud Shell, what should you do?
   
3. (True / False) Cloud Marketplace lets you easily set up services with software that comes packaged with predefined machine types and networks for one-click installation.

4. (True / False) After launching a Marketplace package, you can still change the settings if you want.

5. What's the difference between an internal IP address and an external IP address?

6. Why would developers use internal IP addresses for communication between VMs in the same GCP region, instead of external IP addresses, when both options can achieve the same goal?

7. (True / False) Every VM created in GCP will be automatically binded with an external IP address.

### Week 8

1. Please name a few common network design techniques you can use in Google Cloud.

2. Please briefly describe VM Lifecycles.

3. (True or False) "A terminated VM instance will not incur charges."

4. Why are VM instances divided into many different categories? What's the purpose of that?

5. John Cenee is looking for a cost-effective VM solution for his large-scale, computation-intensive tasks. Which VM type would you recommend? Please justify your answer.

6. Trumpee is looking for an economical VM solution to calculate basic math for his tariffs. While we suggest doing the calculations on his own, he insists on using VMs. His workload fluctuates significantly, with sudden spikes in demand followed by long periods of inactivity. What kind of VM type and configuration would you recommand? Please justify your answer.

7. Kamalee is looking for a VM solution for her predictable, consistent workload with an expected 2-year usage period. She's on a tight budget. Is there any way to get a discounted price? Please explain your answer.