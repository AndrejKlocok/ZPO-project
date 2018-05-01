ZPO-project - Rotace obrazu
Popis

GUI aplikácia pre rotáciu obrázka využívajúca rôzne metódy rotácia a interpolácie.
Návod na zostavenie
	Vhodná distribúcia open source knižnice OpenCV. Potrebné knižnice: -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs
	Nástroj qmake spolu s distribúciou najnovšieho Qt.

Sestavení
	qmake ZPO-project.pro
	make
	./rotation
Pro vyzkoušení aplikace lze využít obrázky přiložené ve složce inputs.