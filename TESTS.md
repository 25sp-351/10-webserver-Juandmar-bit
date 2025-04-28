The server can open port between 1024 and 65535 with the exeption of 80.
When running 80 it will need to command with "sudo ./web_server -p" the word sudo
will give permissions to run on localhost. The only problem with the program it is 
when running something like "localhost/static/dog.png" in this example inside the
http response will not be given or sometime it will take loading. 
The location do not exist and the server can not handle it.
This is the only problem I found so far.