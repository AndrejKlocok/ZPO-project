# ZPO-project - Rotace obrazu

## Popis
GUI aplikácia pre rotáciu obrázka využívajúca rôzne metódy rotácia a interpolácie.

## Návod na zostavenie

1. Vhodná distribúcia open source knižnice [OpenCV](https://docs.opencv.org/). Potrebné knižnice:
	-lopencv_core
	-lopencv_highgui
	-lopencv_imgproc
	-lopencv_imgcodecs

2.  Nástroj qmake spolu s distribúciou najnovšieho [Qt](https://www.qt.io/), [Dokumentácia](http://doc.qt.io/)

## Build
		qmake ZPO-project.pro
		make
		./rotation
