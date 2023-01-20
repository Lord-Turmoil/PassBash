# PassBash

Copyright © Tony's Studio 2020 - 2023

-----

## Try It out!

This is an open source project. Check it out on Gitee and Github!

- [Gitee](https://gitee.com/tonys-studio/pass-bash)
- [Github](https://github.com/Lord-Turmoil/PassBash) [ Currently unavailable :( ]

You can also take a look at my blog - [PassBash](http://www.tonys-studio.top/2023/01/18/PassBash/). :)

## What Is It?

PassBash, a.k.a. Pash, is a light-weight command-line password manager, using Tiny Encryption Algorithm to protect your data. And, again, developed with [tinyxml2](http://leethomason.github.io/tinyxml2/).

It provides you with management abilities to tidy your passwords in groups and items. Each password item is like a "file" in your computer, and you can put related passwords into one group, representing a folder. Of course, you can create groups in groups.

## How to Use It?

Just use it like Linux bash, but with fewer functions. Basically, I tried to make the meaning of commands in PassBash the same as what is Linux. You can always use help to check available commands.

Below is a simple example of the commands in PassBash.

```bash
.$ mkdir social
Group "social" created.
.$ cd social
./social$ touch qq
Password item "qq" created.
./social$ mod qq
------------------------------------------- Edit Mode -------------------------------------------
Use "help" for more information.
 qq> see
                                             Nothing
 qq> set Username|Nick Name
  ID |                  Key |                Value |       Weight
   0 |             Username |            Nick Name |            4
 qq> set Password|123456
  ID |                  Key |                Value |       Weight
   0 |             Username |            Nick Name |            4
   1 |             Password |               123456 |            8
 qq> set Comment|This is a demo
  ID |                  Key |                Value |       Weight
   0 |             Username |            Nick Name |            4
   1 |             Password |               123456 |            8
   2 |              Comment |       This is a demo |           12
 qq> quit

------------------------------------------- Edit End -------------------------------------------
./social$ cat qq
  ID |                  Key |                Value |       Weight
   0 |             Username |            Nick Name |            4
   1 |             Password |               123456 |            8
   2 |              Comment |       This is a demo |           12
./social$ ls
Total 1 groups and items:
        qq
./social$ rename qq QQ
Rename "./social/qq" to "./social/QQ".
./social$ ls
Total 1 groups and items:
        QQ
./social$ cd ..
.$ find qq
Total 1 result:
        ./social/QQ
```

