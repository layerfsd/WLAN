Atheros Communications
AR6001 WLAN Driver for SDIO installation Read Me
March 26,2007

(based on k14 fw1.1)

Windows CE Embedded CE 6.0 driver installation.


1. Unzip the installation file onto your system (called installation directory below)
2. Create an OS design or open an existing OS design in Platform Builder 6.0. 
   a. The OS must support the SD bus driver and have an SD Host Controller driver (add these from Catalog Items).
   b. Run image size should be set to allow greater than 32MB. 
3. a. From the Project menu select Add Existing Subproject...
   b. select AR6K_DRV.pbxml
   c. select open
   This should create a subproject within your OS Design project for the AR6K_DRV driver.
4. Build the solution.





