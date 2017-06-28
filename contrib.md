# Contributors
This book was built on a lot of hard work, most of which happened elsewhere. Without permission to republish from these folks, this book would still be in development. They have our gratitude and admiration.

So, reader, when you’ve completed a few CTFs, and you’re ready for more, reach out to this list. They like to support ambition, and they just might know someone who needs some talent.

* [Andrew Ruef](http://www.mimisbrunnr.net/~munin/blog/) created the initial proposal and slides
* [Evan Jensen](https://github.com/wontonSlim) developed and refined nearly all of the lessons
* [Nick Anderson](https://github.com/PoppySeedPlehzr) ran through the lessons and made numerous improvements
* [Alex Sotirov](http://www.phreedom.org/) delivered the ROP lecture and provided feedback
* [Jay Little](https://twitter.com/computerality) reviewed the binary exploitation modules
* [Brandon Edwards](https://twitter.com/drraid) delivered the source auditing lectures and the newspaper app
* [Marcin W](https://twitter.com/marcinw) and [Gotham Digital Science](http://www.gdssecurity.com/) delivered the web security lectures
* [Dino Dai Zovi](http://www.theta44.org/main.html) delivered the introductory exploitation lectures
* [Mike Myers](https://twitter.com/fristle) for contributing the chapter on forensics

If you're interested in taking a course like this one for credit, check out [NYU Poly](http://engineering.nyu.edu/academics/departments/computer/). They offer concentrations in cybersecurity and we collaborate with them frequently through their [Hacker in Residence](http://www.isis.poly.edu/hackers-in-residence) program.

# Contributing
If you want to make a contribution, simply commit your new markdown to the `master` branch and we'll take it from there. Gitbook has a fantastic [editor](https://github.com/GitbookIO/editor/releases) available to help preview your changes. We're always looking for new or refined content, so please send us your suggestions!

## Gitbook Usage
The CTF Field Guide is built with [Gitbook](https://github.com/GitbookIO/gitbook), a command line tool for building books with Git and Markdown. You can use Gitbook to output the CTF Field Guide as a PDF, an eBook or a single, printable HTML page. Make sure you have [NodeJS](http://nodejs.org/) and `npm` on your operating system, then install Gitbook and a few of its plugins:

```
npm install gitbook gitbook-plugin-ga gitbook-pdf ebook-convert -g
```

With Gitbook installed, you can run any of these commands from within the book directory:
* Generate an interactive, static website: ```gitbook build ./myrepo```
* Generate a single page website: ```gitbook build ./myrepo -f page```.
* Generate a PDF: ```gitbook pdf ./myrepo```. Requires [gitbook-pdf](https://github.com/GitbookIO/gitbook-pdf).
* Generate an eBook: ```gitbook ebook ./myrepo```. Requires [ebook-convert](http://manual.calibre-ebook.com/cli/ebook-convert.html).
