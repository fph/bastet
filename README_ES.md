 TRANSALTION TO SPANISH LANGUAGE
you should follow the following steps. 

1.INSTALL gettext

http://archive.linuxfromscratch.org/lfs-museum/5.1-pre1/LFS-BOOK-5.1-PRE1-HTML/chapter06/gettext.html

2. 

# xgettext --keyword=_ --language=C --add-comments --sort-output -o main.pot main.cpp

3. 

# msginit -l es_AR.UTF-8 -o traslate/es_AR/main.po -i main.pot

4. 
  
# msgfmt -c -v -o traslate/es_AR/main.mo traslate/es_AR/main.po

5.
# sudo cp -v main.mo /usr/share/locale/es_AR/LC_MESSAGES/
6.

Insert the following lines in all the functions that there is text on 
 
setlocale(LC_ALL, "");

bindtextdomain("main", "/usr/share/locale");

textdomain("main");
