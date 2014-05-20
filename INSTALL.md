# Contributing
If you want to make a contribution, simply commit your new markdown to the `master` branch and we'll take it from there. The instructions below are only for setting up your own, full development environment and are uneccessary for most people.

## Dependencies
You're going to need [NodeJS](http://nodejs.org/), [Gitbook](https://github.com/GitbookIO/gitbook), and one of its [plugins](https://github.com/GitbookIO/plugin-ga). Once you have NodeJS and `npm`, run `npm install gitbook gitbook-plugin-ga -g`. After that, you should be able to checkout the [trailofbits/ctf](https://github.com/trailofbits/ctf) repository and run `gitbook serve` for a local webserver. If you want to get fancy, there is a GUI editor available in [GitbookIO/editor](https://github.com/GitbookIO/editor).

## Git Branches
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

## Updating the Website
When you're ready to update the website, commit your new and updated markdown to the master branch as normal. Then run the `to_gh.sh` script to have gitbook build the book and copy it into the `ctf-pages` directory. Move over to the `ctf-pages` directory and run `git_yolo.sh` there (I'm great at git, I know). This will commit the new, compiled website to the gh-pages branch and update the website.
