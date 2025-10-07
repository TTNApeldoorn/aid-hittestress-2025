Er zijn helaas toch nog wat PCB wijzigingen.
De 5V pin van de LilyGO T-Beam V1.2 is nu zowel input als output en is gekoppeld met de USB 5V. Dus de micro-USB stekker kan hiermee vervallen.
Deze 5V pin geeft geen output meer als de sensor op batterij draait, dat was bij de oude versie wel zo.
Dus nu moet de input pin van de step-up (nodig voor de SDS10) aan de 3V3 van de T-Beam aangesloten worden i.p.v. de 5V output.
Ik heb dit aangepast met 2 draadjes en een spoortje doorkrassen, zie foto.