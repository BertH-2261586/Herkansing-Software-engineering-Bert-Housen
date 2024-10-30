# Fully dressed use case maak vragen
**Use case:** Maak vraag

**Scope:** Offline deel van Leerhulpmiddel

**Level:** User goal

**Primary Actor:** Student of Docent

**Stakeholders and interests:**
* Student: Wilt naderhand de leerstof leren op basis van de gemaakte vragen.
* Docent: Wilt vragen maken, zodat zijn studenten deze kunnen gebruiken tijdens hun studies.

**Preconditions:** De gebruiker heeft minstens 1 vragenset.

**Postconditions:** De gemaakte vraag is opgeslagen. Er wordt een online back-up gemaakt als de gebruiker ingelogd doen.

**Main Success Scenario:**

1. Gebruiker heeft een vraag die hij in de software wil invullen.
2. Gebruiker navigeert naar de vragenset waar zij deze vraag in wil zetten
3. Gebruiker selecteert de optie om de vraag aan te maken in de vragenset.
4. Gebruiker selecteert het type vraag: flashcard, meerkeuzevraag of invulvraag.
5. Gebruiker voert de vraag gegevens in
6. Het systeem verwerkt deze elementen en valideert de dat ze correct zijn ingesteld.
7. De gebruiker kiest waar ze dit willen opslaan: als online backup, lokaal of op beide manieren.
8. Het systeem slaat de vraag op in de database.
9. Bij succesvolle opslag ontvangt de gebruiker een bevestiging.
10. De vraag is beschikbaar onder de vragenset die de gebruiker had geselecteerd en is klaar voor gebruik.

**Uitbreidingen (Alternatieve Stromen)**

5a. Als de vraag een flashcard is: 
1. de gebruiker vult de "Vraag" en "Antwoord" velden in.

5b. Als de vraag een meeurkeuze vraag is: 
1. de gebruiker vult de vraagtekst, meerdere antwoordopties en het juiste antwoord(en) in.

5c. Als de vraag een invulvraag is: 
1. de gebruiker vult de “Vraag” en “Antwoord” velden in. In het vraag veld kan de gebruiker verschillende variabelen invoegen voor een meer dynamisch antwoord. Je kan hier dan een bereik aan geven, zodat het systeem tijdens een ondervraging hier dynamisch een waarde aan geeft.

5d. Gebruiker slaat variabele elementen over:
1. Het systeem toont een foutmelding en vraagt de gebruiker om de ontbrekende elementen in te vullen.

6a. Ongeldige of Onvolledige Invoer:
1. Het systeem toont een foutmelding en vraagt de gebruiker om de ontbrekende of ongeldige velden aan te passen.

7a. De gebruiker is offline:
1. De gebruiker kan de vraag dan alleen lokaal opslaan.

9a. Opslagfout (Bv. netwerkinstabiliteit tijdens het opslaan):
1. Het systeem geeft een foutmelding en de gebruiker moet vanaf stap 7 verder werken.

**Special Requirements:** Het maken van een invulvraag mag niet verwarrend zijn voor de gebruiker.

**Technology and Data Variations List:** Geen

**Frequency of Occurences:** Dit wordt redelijk frequent uitgevoerd door studenten en docenten die vragen willen maken om te willen leren/delen.
