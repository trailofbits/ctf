# Contributors
This book was built on a lot of hard work, most of which happened elsewhere. Without permission to republish from these folks, this book would still be in development. They have our gratitude and admiration.

So, reader, when you’ve completed a few CTFs, and you’re ready for more, reach out to this list. They like to support ambition, and they just might know someone who needs some talent.

* [Andrew Ruef](https://web.archive.org/web/20150219123305/http://www.mimisbrunnr.net:80/~munin/blog/) created the initial proposal and slides
* Evan Jensen developed and refined nearly all of the lessons
* [Nick Anderson](https://github.com/PoppySeedPlehzr) ran through the lessons and made numerous improvements
* [Alex Sotirov](http://www.phreedom.org/) delivered the ROP lecture and provided feedback
* [Jay Little](https://x.com/computerality) reviewed the binary exploitation modules
* [Brandon Edwards](https://x.com/drraid) delivered the source auditing lectures and the newspaper app
* [Marcin W](https://x.com/marcinw) and [Gotham Digital Science](https://www.gdssecurity.com/) delivered the web security lectures
* [Dino Dai Zovi](https://theta44.org/) delivered the introductory exploitation lectures
* [Mike Myers](https://x.com/fristle) for contributing the chapter on forensics

If you're interested in taking a course like this one for credit, check out [NYU Poly](https://engineering.nyu.edu/academics/departments/computer-science-and-engineering). They offer concentrations in cybersecurity and we collaborate with them frequently through their [Hacker in Residence](https://web.archive.org/web/20170409054057/http://www.isis.poly.edu/hackers-in-residence) program.

# Contributing
If you want to make a contribution, simply commit your new markdown to the `master` branch and we'll take it from there. We're always looking for new or refined content, so please send us your suggestions!

## mdBook Usage
The CTF Field Guide is built with [mdBook](https://rust-lang.github.io/mdBook/), a command line tool for building books with Git and Markdown. Refer to [mdBook's installation page](https://rust-lang.github.io/mdBook/guide/installation.html) to get it set up.

With mdBook installed, you can run any of these commands from within the book directory:
* Generate an interactive, static website: ```mdbook build```
* Serve the book via HTTP, to locally browse and edit it: ```mdbook serve```.

Third-party plugins also exist if you wish to export the book as EPUB or PDF.
