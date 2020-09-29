
.. figure:: Art/logo.png

**CS Systèmes d'Information**

| Business Unit ESPACE                                                                                          
| Département Payload Data & Applications    
| Image Processing		   


.. tabularcolumns:: |L|J|J|J|p{0.6in}|

=============================================== == ================= ============
Software: **MAJA**
**SETG-**\ **MU-**\ **MAJA-**\ **010-**\ **CS**                       
Change                                          02 Date              29/11/2019
Issue                                           02 Date              05/05/2020
Distribution Code                               E
Ref. : CSSI/SPACE/PD&A/MAJA/MU 
=============================================== == ================= ============

**USER, INSTALLATION AND OPERATING MANUAL FOR MAJA CHAINS [MU]**

=================================== =================
Written by :                        Date : 05/05/2020 
                                                    
ESQUIS Benjamin CSSI/ESPACE/PDA/IP                   
BRICIER Aurélien CSSI/ESPACE/PDA/IP                  
Approved by :                       Date : 05/05/2020 
                                                     
OLIVIE Francis CSSI/ESPACE/DSM                       
For application :                   Date : 05/05/2020
                                                     
Esquis Benjamin CSSI/ESPACE/PDA/IP                   
=================================== =================

.. tabularcolumns:: |L|L|p{1in}|p{1in}|
		    
+----------------------------------------------------------------------------------------+-----------------------------------------------------+------------------------+---------------+
| CONFIDENTIALITY :                                                                      | KEYWORDS : MAJA; MI, MU, ME; STAND-ALONE                                                     |
| DLP                                                                                    |                                                                                              |
+----------------------------------------------------------------------------------------+-----------------------------------------------------+------------------------+---------------+
| TITLE :                                                                                                                                                                               |
| USER, INSTALLATION AND OPERATING MANUAL                                                                                                                                               |
| MAJA CHAINS                                                                                                                                                                           |
| [MU]                                                                                                                                                                                  |
+----------------------------------------------------------------------------------------+-----------------------------------------------------+------------------------+---------------+
|   AUTHOR(s) :                                                                          | | BRICIER Aurélien CSSI/ESPACE/PDA/IP                                                        |
|                                                                                        | | ESQUIS Benjamin  CSSI/ESPACE/PDA/IP                                                        |
+----------------------------------------------------------------------------------------+-----------------------------------------------------+------------------------+---------------+
| SUMMARY : This is MAJA user, installation and operating manual in a stand-alone context.                                                                                              |
+----------------------------------------------------------------------------------------+-----------------------------------------------------+------------------------+---------------+
| RELATED DOCUMENTS : Stand alone document.                                              | LOCALIZATION :                                                                               |
|                                                                                        | CSSI/SPACE/PD&A/MAJA                                                                         |
+----------------------------------------------------------------------------------------+-----------------------------------------------------+------------------------+---------------+
| Volume : 1                                                                             | TOTAL NUMBER OF PAGES : 60                          | COMPOSITE DOCUMENT : N | LANGUAGE : EN |
|                                                                                        |                                                     |                        |               |
|                                                                                        | INCLUDING PRELIMINARY PAGES : 0                     |                        |               |
|                                                                                        |                                                     |                        |               |
|                                                                                        | NUMBER OF SUPPL. PAGES : 0                          |                        |               |
+----------------------------------------------------------------------------------------+-----------------------------------------------------+------------------------+---------------+
| CONFIGURATION MANAGEMENT : NG                                                          | CM RESP. :                                                                                   |
+----------------------------------------------------------------------------------------+-----------------------------------------------------+------------------------+---------------+
| CAUSE D'EVOLUTION : Update document for MAJA v4.2.0                                                                                                                                   |
+----------------------------------------------------------------------------------------+-----------------------------------------------------+------------------------+---------------+
| HOST SYSTEM : WordVersion                                                                                                                                                             |
+----------------------------------------------------------------------------------------+-----------------------------------------------------+------------------------+---------------+

**Internal Distribution :**

================== ==================== =================== ============
Name               Entity               Internal Postal Box Observations
OLIVIE Francis     CSSI/ESPACE/DSM                         
RECULEAU Serge     CSSI/ESPACE/PDA/PDGS                    
BROSSARD julie     CSSI/ESPACE/PDA/IP                      
ESQUIS Benjamin    CSSI/ESPACE/PDA/IP                      
\                                                          
\                                                          
\                                                          
\                                                          
\                                                          
\                                                          
\                                                          
\                                                          
\                                                          
================== ==================== =================== ============

**External Distribution :**

=================== ======================== ============
Name                Entity                   Observations
BAILLARIN Simon     DNO/OT/IS                
HAGOLLE Olivier     DSO/SI/CB                
KETTIG Peter        DNO/OT/IS                
LARIF Marie-France  DNO/OT/PE                
SELLE Arnaud        DNO/OT/TA                
PACHOLCZYK Philippe DNO/OT/TA                
PUJOL Mathilde      DNO/DA/AQ                
\                                            
=================== ======================== ============

**Changes history :**

.. tabularcolumns:: |p{1cm}|L|p{2cm}|L|
		    
===== ==== ========== ===============================================================================================
Issue Rev. Date       Reference, Author(s), Reasons for evolution
02    02   05/05/2020  CSSI/SPACE/PD&A/MAJA/MU
                     
                      ESQUIS Benjamin CSSI/ESPACE/PDA/IP
                     
                      Update document for MAJA v4.2.0
02    01   10/03/2020  CSSI/SPACE/PD&A/MAJA/MU
                     
                      ESQUIS Benjamin CSSI/ESPACE/PDA/IP
                     
                      Update document for MAJA v4.1.0
02    00   29/11/2019  CSSI/SPACE/PD&A/MAJA/MU
                     
                      ESQUIS Benjamin CSSI/ESPACE/PDA/IP
                     
                      Update document for MAJA v4.0.0
01    04   24/06/2019  CSSI/SPACE/PD&A/MAJA/MU
                     
                      ESQUIS Benjamin CSSI/ESPACE/PDA/IP
                     
                      Update document for MAJA v3.3.1
01    03   18/04/2018  CSSI/SPACE/PD&A/MAJA/MU
                     
                      BRICIER Aurélien CSSI/ESPACE/PDA/IP
                     
                      Update document for MAJA v3.1.0
01    02   19/10/2017  CSSI/SPACE/PD&A/MAJA/MU
                     
                      ESQUIS Benjamin CSSI/ESPACE/PDA/IP
                     
                      Update document for MAJA v3.0.0.
01    01   26/06/2017  CSSI/SPACE/PD&A/MAJA/MU
                     
                      FEUVRIER Thomas CSSI/ESPACE/PDA/IP
                     
                      Update document for MAJA v2.1.0. Add installation prerequisites for 'gzip' and 'bzip2' packages
01    00   20/01/2017  CSSI/SPACE/PD&A/MAJA/MU
                     
                      FEUVRIER Thomas CSSI/ESPACE/PDA/IP
                     
                      Previous document reference LAIG-MU-MAC-010-CS Ed.3 Rev.12
                     
                      Creation of document for MAJA v1.0
\                    
\                    
===== ==== ========== ===============================================================================================

Table of contents
=================

.. toctree::
	:maxdepth: 5
	:caption: Glossary and list of TBC and TBD items
	
	Glossary_TBD

.. toctree::
	:maxdepth: 5
	:caption: Reference And applicable documents
	
	Applicable_doc

.. toctree::
	:maxdepth: 5
	:caption: SYSTEM REQUIREMENTS
	
	System_requirements

.. toctree::
	:maxdepth: 5
	:caption: Installation manual
	
	Installation

.. toctree::
	:maxdepth: 5
	:caption: User manual
	
	User_Manual

.. toctree::
	:maxdepth: 5
	:caption: Operating manual
	
	Operating_Manual

