# Appendix

## Contributing
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

## Development
The instructions below are only for setting up your own, full development environment and are uneccessary for most people.

### Git Branches
The CTF Field Guide is built on GitHub Pages and, therefore, there are two branches: master, for the raw markdown, and gh-pages, for the compiled website. Here's how I manage both of them on my own system.

First, clone the master branch into a folder like normal.

```
dan:dan $ cd ~/github
dan:github $ mkdir ctf
dan:github $ cd ctf
dan:github $ git clone https://github.com/trailofbits/ctf .
```

Next, clone the gh-pages branch into another folder inside the same parent. In this directory, switch to the gh-pages branch and delete the master branch.

```
dan:dan $ cd ~/github
dan:github $ mkdir ctf-pages
dan:github $ cd ctf-pages
dan:ctf-pages $ git clone https://github.com/trailofbits/ctf .
dan:ctf-pages $ git checkout origin/gh-pages -b gh-pages
dan:ctf-pages $ git branch -d master
dan:ctf-pages $ git branch
* gh-pages
```

Now you have one folder to track each branch, `master` and `gh-pages`.

### Updating the Website
When you're ready to update the website, commit your new and updated markdown to the master branch as normal. Then run the `to_gh.sh` script to have gitbook build the book and copy it into the `ctf-pages` directory. Move over to the `ctf-pages` directory and run `git_yolo.sh` there (I'm great at git, I know). This will commit the new, compiled website to the gh-pages branch and update the website.
