## Fully dressed use case
**Use case:** Voeg docent toe aan vak

**Scope:** Applicatie GUI en backend

**Level:** User goal

**Primary Actor:** Docent

**Stakeholders and interests:**
* Docent: wil docenten toevoegen aan het onderwijsteam van een vak

**Preconditions:** 
1. Vak dient aangemaakt te zijn
2. Docent dient deel te zijn van het onderwijsteam van het vak om andere toe te voegen

**Postconditions:** Nieuwe docent is toegevoegd aan het vak

**Main Success Scenario:**
1. De docent wilt andere docenten toevoegen aan het vak
2. De user gaat naar het vakscherm
3. De user navigeert naar het gewilde vak
4. De user selecteert de optie om docenten toe te voegen
5. De user kiest de docent uit de lijst met alle docenten
6. De user voegt de docent toe

**Uitbreidingen (Alternatieve Stromen):**   
6a. Er gaat iets mis met het toevoegen:
1. De user wordt vermeld dat er iets is fout gegaan

**Special Requirements:** 


**Technology and Data Variations List:** Geen

**Frequency of Occurences:** Laag, onderwijsteams veranderen niet vaak

## Systeemsequentiediagram
![Systeem Sequentie Diagram](./Images/SSD_voeg_docent_toe_aan_vak.png)

## Contracten
### **Naam:** Open vakscherm<br/>
**Cross References:** 
* **Use cases:** Voeg docent toe aan vak<br/>
* **SSD’s:** Voeg docent toe aan vak
  
**Pre-condities:** De gebruiker moet ingelogd zijn op een docent account<br/>
**Post-condities:** 
* **Instance creation:** Een vakscherm instantie wordt aangemaakt

---

### **Naam:** Selecteer het gewenste vak<br/>
**Cross References:** 
* **Use cases:** Voeg docent toe aan vak<br/>
* **SSD’s:** Voeg docent toe aan vak
  
**Pre-condities:** Er moet minstens 1 vak zijn waar de docent toegang toe heeft<br/>
**Post-condities:** 
* **Instance destruction:** Het vakscherm wordt gesloten
* **Instance creation:** Een scherm voor het specifieke vak wordt aangemaakt

---

### **Naam:** Selecteer docent toevoegen aan onderwijsteam<br/>
**Cross References:** 
* **Use cases:** Voeg docent toe aan vak<br/>
* **SSD’s:** Voeg docent toe aan vak
  
**Pre-condities:** Het scherm voor het specifieke vak moet aangemaakt zijn<br/>
**Post-condities:** 
* **Instance creation:** Een scherm voor docenten toe te voegen wordt getoond

---

### **Naam:** Presenteer alle docenten<br/>
**Cross References:** 
* **Use cases:** Voeg docent toe aan vak<br/>
* **SSD’s:** Voeg docent toe aan vak
  
**Pre-condities:** Het scherm om docenten toe te voegen moet getoond zijn<br/>
**Post-condities:** 
* **Attribute modification:** Er wordt een lijst in het scherm om docenten toe te voegen gezet waar alle docenten in staan

---

### **Naam:** Selecteer de gewenste docent<br/>
**Cross References:** 
* **Use cases:** Voeg docent toe aan vak<br/>
* **SSD’s:** Voeg docent toe aan vak
  
**Pre-condities:** Er moet minstens 1 docent beschikbaar zijn die niet die nog niet is toegevoegd<br/>
**Post-condities:** 
* **Attribute modification:** De geselecteerde wordt tijdelijke opgeslagen

---

### **Naam:** Vul de titel/rol in van de docent<br/>
**Cross References:** 
* **Use cases:** Voeg docent toe aan vak<br/>
* **SSD’s:** Voeg docent toe aan vak
  
**Pre-condities:** Er moet een docent geselecteerd zijn<br/>
**Post-condities:** 
* **Attribute modification:** De titel word tijdelijk opgeslagen

---

### **Naam:** Geef correcte feedback<br/>
**Cross References:** 
* **Use cases:** Voeg docent toe aan vak<br/>
* **SSD’s:** Voeg docent toe aan vak
  
**Pre-condities:** Er moet een docent geselcteerd zijn en een titel ingevuld en dit moet bevestigd zijn<br/>
**Post-condities:** 
* **Instance creation:** Als er iets mis gaat of een veld niet goed is ingevuld wordt de gebruiker hiervan verwittigd
* **Instance deletion:** Het scherm voor docenten toe te voegen wordt gesloten als er geen fouten zijn

---

## UML Diagrammen ontwerp
![Voeg docent toe sequentie diagram](./Images/DocentToevoegen.png)

## Klassen diagram
![Klassediagram](./Images/Klasse_Diagram_leerhulpmiddel_It2.png)
