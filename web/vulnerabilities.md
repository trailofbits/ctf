# Auditing Web Applications
Welcome to the web hacking module. This module is about getting familiar with vulnerabilities commonly found in web applications. At the end of this module you will be able to identify common vulnerabilities in web based applications using a variety of testing methodologies and source level auditing. The lecture material will give you all the tools you need to successfully audit the workshop material.

## Lecture
* [Web Hacking Part I](http://vimeo.com/32509769)
* [Web Hacking Part II](http://vimeo.com/32550671)

## Workshop
In order to practice your skills, we recommend going through the process of finding and exploiting vulnerabilities in the [Damn Vulnerable Web App (DVWA)](http://www.dvwa.co.uk/) and the Siberia Exploit Kit. DVWA is a collection of vulnerable test cases implemented in PHP and serves as an easy introduction to the many things that can go wrong in web applications. The Siberia Exploit Kit is a "crimeware pack" used by criminals to perform massive compromises. It includes a package of browser exploits and a command and control panel intended to manage compromised hosts. Siberia contains several pre- and post-authentication vulnerabilities that allow an attacker to gain administrative access to the panel, then take over the server on which it is hosted.

Download and run the [OWASP Broken Web Apps](https://code.google.com/p/owaspbwa/) virtual machine in VMware to start this workshop. BWA includes many web applications many for security testing, including DVWA. Once you have mastered DVWA, feel free to move on to other vulnerable web applications! Try auditing Siberia's source code to find the vulnerabilities, paying attention to sources of input in PHP.

* [OWASP Broken Web Apps](https://code.google.com/p/owaspbwa/)
* [Siberia Crimeware Pack](./workshop/siberia.zip) (pw: infected)

## Tools
Burp Suite is a local HTTP proxy intended for security testing. Burp Suite is made for web penetration testers and simplifies many common tasks in a point-and-click GUI. The features available in the free version are more than enough to complete this and many other web security challenges.
* [Burp Suite](http://portswigger.net/burp/download.html)

## Resources
Many simple testing methods and common web application flaws are available in the walkthrough. Ensure that you understand the fundementals of HTTP, HTML, and PHP to do well on this section.
* [OWASP Top 10 Tools and Tactics](http://resources.infosecinstitute.com/owasp-top-10-tools-and-tactics/)
* [The Tangled Web: Chapter 3](http://www.nostarch.com/download/tangledweb_ch3.pdf)
* [PHP Primer](http://www2.astro.psu.edu/users/sdb210/documents/phpprimer_v0.1.pdf)
