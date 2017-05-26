# diary
A very basic diary application written in C++ and Qt. It keeps its data in an SQLite database. 
Encrypts the text only to keep it safe from your 9 year old sister.

Some people may not want a login screen and some may not want even the basic encryption.
I added an option to keep the diary unencrypted.

If you supply a password, program will use it to encrypt entries. So, if you forget your password you may say your diary goodbye.
Password protection works like this: First the password is hashed, using SHA512. Then resulting hash will be hashed with the entry key.
Entry key is automatically produced by SQLite during the entry creation. Every entry has a different key used like salt.

Now it is using AES 128 bit encryption (https://github.com/kokke/tiny-AES128-C). I just used SHA512 hash + user's password hash as entry key. Reduced 512 bit hash to 128 bit key.

Added key files support just like Veracrypt. This option keeps the data much more safer because keyloggers can record your keyboard but cannot know which jpeg or mp3 files you use to encrypt the data.
Key files can be in any order. Please not that first 1024 bytes of a key file is used and must not be changed. Carefully select your key files. You can use pictures, music files etc. as well as they are not modified.

