# Google Cloud Skills Boost Google Cloud

## 02 Google Cloud Fundamentals: Core Infrastructure

### Introducing Google Cloud

1. What type of cloud computing service lets you bind your application code to libraries that give access to the infrastructure your application needs?
   - [ ] Virtualized data centers
   - [ ] Platform as a service
   - [ ] Infrastructure as a service
   - [ ] Software as a service

1. Why might a Google Cloud customer use resources in several regions around the world?
    - [ ] To improve security
    - [ ] To offer localized application versions in different regions
    - [ ] To earn discounts
    - [ ] To bring their applications closer to users around the world, and for improved fault tolerance 

2. What is the primary benefit to a Google Cloud customer of using resources in several zones within a region?
    - [ ] For getting discounts on other zones
    - [ ] For improved fault tolerance
    - [ ] For expanding services to customers in new areas
    - [ ] For better performance

### Resources and Access in the Cloud
1. Choose the correct completion: Services and APIs are enabled on a per-________ basis.
   - [ ] Organization
   - [ ] Project
   - [ ] Billing account
   - [ ] Folder

2. Which of these values is globally unique, permanent, and unchangeable, but can be modified by the customer during creation?
    - [ ] The project name
    - [ ] The project's billing credit-card number
    - [ ] The project number
    - [ ] The project ID 

3. Order these IAM role types from broadest to finest-grained.
    - [ ] Predefined roles, custom roles, basic roles
    - [ ] Basic roles, predefined roles, custom roles
    - [ ] Custom roles, predefined roles, basic roles

### Getting Started with VPC Networking and Google Compute Engine
1. Which firewall rule allows the ping to mynet-r2-vm's external IP address?
    - [ ] mynetwork-allow-ssh
    - [ ] mynetwork-allow-icmp
    - [ ] mynetwork-allow-rdp
    - [ ] mynetwork-allow-custom

2. How does Cloud Load Balancing allow you to balance HTTP-based traffic?
   - [ ] Across multiple physical machines in a single data center.
   - [ ] Across multiple virtual machine instances in a single Compute Engine region.
   - [ ] Across multiple Compute Engine regions.
   - [ ] Across multiple Google Cloud Platform services.

3. For which of these interconnect options is a Service Level Agreement available?
    - [ ] Standard Network Tier
    - [ ] Direct Peering
    - [ ] Dedicated Interconnect
    - [ ] Carrier Peering

### Storage in the Cloud
1. What is the correct use case for Cloud Storage?
    - [ ] Cloud Storage is well suited to providing durable and highly available object storage.
    - [ ] Cloud Storage is well suited to providing RDBMS services.
    - [ ] Cloud Storage is well suited to providing data warehousing services.
    - [ ] Cloud Storage is well suited to providing the root file system of a Linux virtual machine.

2. Why would a customer consider the Coldline storage class?
    - [ ] To save money on storing frequently accessed data.
    - [ ] To improve security.
    - [ ] To use the Coldline Storage API.
    - [ ] To save money on storing infrequently accessed data.

3. Which relational database service can scale to higher database sizes?
    - [ ] Spanner
    - [ ] Firestore
    - [ ] Bigtable
    - [ ] Cloud SQL

#### Containers in the Cloud

1. What is a Kubernetes pod?
    - [ ] A group of nodes
    - [ ] A group of clusters
    - [ ] A group of containers
    - [ ] A group of VMs

1. Where do the resources used to build Google Kubernetes Engine clusters come from?
   - [ ] Bare-metal servers
   - [ ] App Engine
   - [ ] Compute Engine
   - [ ] Cloud Storage

### Applications in the Cloud
**Multi Choose**
1. Which of these statements about Cloud Run functions are correct? Select three. 'Cloud Run functions:'
    - [ ] Is a scalable functions-as-a-service platform.
    - [ ] Is integrated with Cloud Logging.
    - [ ] Require servers or VMs to be provisioned.
    - [ ] Can be used to extend Cloud services.
    - [ ] Can only be invoked by sending HTTP requests.

2. Why might a Google Cloud customer choose to use Cloud Run functions?
    - [ ] Their application contains event-driven code that they don't want to provision compute resources for.
    - [ ] Their application has a legacy monolithic structure that they want to separate into microservices.
    - [ ] Cloud Run functions is a free service for hosting compute operations.
    - [ ] Cloud Run functions is the primary way to run C++ applications in Google Cloud.

3. Which scenario is best suited for using Cloud Run instead of Cloud Run functions?
   - [ ] Sending an email notification whenever a new document is added to a specific folder in Cloud Storage.
   - [ ] Resizing images on demand when requested by a user through a web interface.
   - [ ] Hosting a dynamic web application that allows users to upload and share photos.
   - [ ] Generating thumbnails for images uploaded to a Cloud Storage bucket.

### Prompt Engineering

1. Which of the following is not a good practice for constructing prompts?
    - [ ] Avoid ambiguity.
    - [ ] Vary your prompts.
    - [ ] Include long and detailed sentences.
    - [ ] Keep it short and simple.

2. Which one of the following is not a type of prompt?
   - [ ] Zero-shot
   - [ ] One-shot
   - [ ] Role prompt
   - [ ] Two-shot

3. How can you avoid hallucinations when using a large language model?
    - [ ] Train the model on a clean, curated dataset.
    - [ ] Train the LLM on a smaller dataset.
    - [ ] Do not add any context to your prompts.
    - [ ] Train the model to operate with very few constraints.

4. Generative AI is a type of artificial intelligence that can ____.
   - [ ] Perform complex calculations and mathematical operations.
   - [ ] Generate responses on real-time data.
   - [ ] Make predictions about future events.
   - [ ] Generate text, images, or other data using generative models.

## 03 Essential Google Cloud Infrastructure: Foundation

### Interacting with Google Cloud

1. To create a persistent state in Cloud Shell, which file would you configure?
    - [ ] .my_variables
    - [ ] .bashrc
    - [ ] .config
    - [ ] .profile

2. What is the difference between the Google Cloud Console and Cloud Shell?
   - [ ] The Cloud Console is a command-line tool, while Cloud Shell is a graphical user interface
   - [ ] Cloud Shell is a locally installed tool, while the Cloud Console is a temporary virtual machine.
   - [ ] There is no difference as these tools are 100% identical.
   - [X] Cloud Shell is a command-line tool, while the Cloud Console is a graphical user interface

3. Which of the following does not allow you to interact with Google Cloud?
    - [X] Cloud Explorer
    - [ ] REST-based API
    - [ ] Cloud Shell
    - [ ] Google Cloud Console

### Vistual Networks

1. Without a VPC network, you cannot create VM instances, containers, or App Engine applications.
    - [ ] True
    - [ ] False

2. Which firewall rule allows the ping to mynet-notus-vm's external IP address?
    - [ ] mynetwork-allow-ssh
    - [ ] mynetwork-allow-icmp
    - [ ] mynetwork-allow-custom
    - [ ] mynetwork-allow-rdp

3. Which instances should you be able to ping from mynet-us-vm using internal IP addresses?
**Multi Choose**
- [ ] mynet-notus-vm
- [ ] managementnet-us-vm
- [ ] privatenet-us-vm

4. Did the command prompt change to `@vm-internal`?
    - [ ] True
    - [ ] False

5. Currently, which of your VM instances can access the image from your bucket?
   - [ ] Cloud Shell
   - [ ] vm-internal

6. What are the three types of networks offered in Google Cloud?
   - [ ] Gigabit network, 10 gigabit network, and 100 gigabit network
   - [ ] IPv4 unicast network, IPv4 multicast network, IPv6 network
   - [ ] Zonal, regional, and global
   - [ ] Default network, auto network, and custom network.

7. What is one benefit of applying firewall rules by tag rather than by address?
   - [ ] Tags on firewall rules control which ephemeral IP addresses VMs will receive.
   - [ ] When a VM is created with a matching tag, the firewall rules apply irrespective of the IP address it is assigned.
   - [ ] Tags in network traffic help with network sniffing.
   - [ ] Tags help organizations track firewall billing.

8. In Google Cloud, what is the minimum number of IP addresses that a VM instance needs?
   - [ ] One: Only an internal IP address
   - [ ] Two: One internal and one external IP address
   - [ ] Three: One internal, one external and one alias IP address

#### Vistual Machines

1. What are sustained use discounts?
    - [ ] Automatic discounts that you get for running specific Compute Engine resources for a significant portion of the billing month
    - [ ] Purchase commitments for specific resources you know you will use
    - [ ] Per-second billing that starts after a 1 minute minimum 
    - [ ] Discounts you receive by using preemptible VM instances
2. Which statement is true of Virtual Machine Instances in Compute Engine?
    - [ ] In Compute Engine, a VM is a networked service that simulates the features of a computer.
    - [ ] All Compute Engine VMs are single tenancy and do not share CPU hardware. 
    - [ ] A VM in Compute Engine always maps to a single hardware computer in a rack.
    - [ ] Compute Engine uses VMware to create Virtual Machine Instances.
3. Which statement is true of persistent disks?
    - [ ] Persistent disks are always HDDs (magnetic spinning disks).
    - [ ] Persistent disks are encrypted by default.
    - [ ] Persistent disks are physical hardware devices connected directly to VMs.
    - [ ] Once created, a persistent disk cannot be resized.