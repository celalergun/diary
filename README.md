# diary
A very basic diary application written in C++ and Qt. It keeps its data in an SQLite database. 
Encrypts the text only to keep it safe from your 9 year old sister.

Some people may not want a login screen and some may not want even the basic encryption.
I added an option to keep the diary unencrypted.

If you supply a password, program will use it to encrypt entries. So, if you forget your password you may say your diary goodbye.
Password protection works like this: First the password is hashed, using SHA512. Then resulting hash will be hashed with the entry key.
Entry key is automatically produced by SQLite during the entry creation. Every entry has a different key.

Now it is using AES 128 bit encryption. I just used SHA512 hash as the encryption key. I know that it uses first 128 bits of it. I will take care of this as soon as possible (key reduction).

TODO: I will add an option to use key files just like Veracrypt. This option keeps the data much more safer because keyloggers can record your keyboard but cannot know which jpeg or mp3 files you use to encrypt the data.
