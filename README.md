# Clouds Detection

Resume
------

This project is about clouds detection in optical satellite images, developed with C programming language.

The folder contain:

* an EXE folder containing your .bmp images files to analyzed
* an OBJ folder containing .o files
* an SRC forder containing
     * sources files
     * Makefile

This project is using an algorithm from the paper "*Introducing spatial information in k-means algorithm for clouds detection in optical satellite images*" written by *L. Beaudoin*, *J.M. Nicolas*, *F. Tupin* and *M. Hückel*.

___
Execution
---------

Go into SRC folder and launch the **make** command:

    > cd SRC/
    > make

That will produce two files, one named **imaProjet.exe** and the other named **imaProjet_auto.exe**. The first one will launch an HMI that will asking you for selecting an *.bmp* image to analized:

    > cd ../EXE/
    > ./imaProjet.exe

You can also run the second *.exe* file that will run automatically all your images from *EXE/* folder and print result algorithm:

    > ./imaProject_auto.exe

___
Authors
-------

EPITA School, SCIA Master 1 - Project for Teledetection course. 

Authors: **BENSABAT David** (bensab_d)

Please, note that **I worked alone** on this project which is normally a project in a team of two. Ideed, I didn't find a partner.
