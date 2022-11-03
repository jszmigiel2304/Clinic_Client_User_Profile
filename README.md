###################################################
# Clinic_Client_User_Profile (https://github.com/jszmigiel2304/Clinic_Client_User_Profile)
Utworzona w Qt/C++, w modelu MVC aplikacja, bêd¹ca modu³em dla Clinic_Client(https://github.com/jszmigiel2304/Clinic_Client) i ze wzglêdu na przekazywane z Clinic_Client informacje, z jej poziomu musi byæ uruchamiana.
Komunikacja z Clinic_Client odbywa siê za pomoc¹ QLocalSocket pod³¹czonego do konkretnego QLocalServer za pomoc¹ identyfikatora otrzymanego z  Clinic_Client.

##########################################################

G³owne klasy aplikacji to:
c_moduleController: to tu inicjalizowane s¹ wszystkie potrzebne klasy,

c_actionExecutive: w tej klasie przetwarzane s¹ wszystkie ¿adania i przygotowywane odpowiedzi

c_clientProcessConnection:klasa odpowiadaj¹ca za komunikacjê z Clinic_Client

--------------------------------------------------------------
Dodatkowo:
c_Parser: tutaj przygotowywane s¹ pakiety do wys³ania do serwera

c_waitingLoop: klasa, która pozwala na wymuszenie oczekiwania do momentu, a¿ aplikacja nie otrzyma ich np serwera. Pomaga w rozwi¹zywaniu problemu asynchronicznegootrzymywania danych z serwera.
waitingCondition: obiekty tej klasy reprezentuj¹ warunki jakie musz¹ byæ spe³nione, ¿eby waitingLoop móg³ przerwaæ oczekiwanie
