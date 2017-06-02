# diary
A very basic diary application written in C++ and Qt. It keeps its data in an SQLite database. 
It uses AES-128 with password stretching (https://www.github.com/kokke/tiny-AES128-C.html). I just used SHA512 hash of user's password and hash of entry key as the encryption key. I reduced 512 bit SHA hash to 128 bit AES key since the algorithm uses 128 bit keys.

If you supply a password, program will use it to encrypt entries. You may choose not to encrypt the entries. If you use password and keyfiles, protect them because if you forget your password or lost your key files you may say your diary goodbye.

Password protection works like this: First the password is hashed, using SHA512. Then resulting hash will be hashed with the entry key.
Entry key is automatically produced by SQLite during the entry creation. Every entry has a different key used like a salt. Then first 1024 bytes of the key files are XOR'ed and added to the hash. File order does not matter since XORing the same bytes in different order gives the same result.

Some people may not want a login screen and some may not want even the basic encryption.
I added an option to keep the diary unencrypted. Just say "No" to the question on first run.

Finally I added key files support just like Veracrypt. This option keeps the data much more safer because keyloggers can record your keyboard but cannot know which jpeg or mp3 files you use to encrypt the data.
Key files can be in any order. Please note that first 1024 bytes of a key file is used and must not be changed. Carefully select your key files and make a backup of these files. You can use pictures, music files etc. as well as they are not modified.

