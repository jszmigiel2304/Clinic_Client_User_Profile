###################################################
# Clinic_Client_User_Profile (https://github.com/jszmigiel2304/Clinic_Client_User_Profile)
Utworzona w Qt/C++, w modelu MVC aplikacja, b�d�ca modu�em dla Clinic_Client(https://github.com/jszmigiel2304/Clinic_Client) i ze wzgl�du na przekazywane z Clinic_Client informacje, z jej poziomu musi by� uruchamiana.
Komunikacja z Clinic_Client odbywa si� za pomoc� QLocalSocket pod��czonego do konkretnego QLocalServer za pomoc� identyfikatora otrzymanego z  Clinic_Client.

##########################################################

G�owne klasy aplikacji to:
c_moduleController: to tu inicjalizowane s� wszystkie potrzebne klasy,

c_actionExecutive: w tej klasie przetwarzane s� wszystkie �adania i przygotowywane odpowiedzi

c_clientProcessConnection:klasa odpowiadaj�ca za komunikacj� z Clinic_Client

--------------------------------------------------------------
Dodatkowo:
c_Parser: tutaj przygotowywane s� pakiety do wys�ania do serwera

c_waitingLoop: klasa, kt�ra pozwala na wymuszenie oczekiwania do momentu, a� aplikacja nie otrzyma ich np serwera. Pomaga w rozwi�zywaniu problemu asynchronicznegootrzymywania danych z serwera.
waitingCondition: obiekty tej klasy reprezentuj� warunki jakie musz� by� spe�nione, �eby waitingLoop m�g� przerwa� oczekiwanie
