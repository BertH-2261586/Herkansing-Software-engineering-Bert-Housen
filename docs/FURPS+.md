### Functionality
<ol>
    <li> Vragen: </li>
        <ol type="i">
            <li> Kan een vraag maken</li>
            <li> Je kan kiezen wat voor soort vraag je wilt maken (meerkeuze, invulvraag, flashcard)</li>
            <li> Kan afbeeldingen, audio, videos toevoegen aan vragen als hulpmiddel</li>
            <li> Kan vragen delen met vrienden </li>
        </ol>
    <li> Ondervragingen: </li>
        <ol>
            <li> Je kan een ondervraging maken op basis van geselecteerde vragen </li>
            <li> Je kan ze customizen </li>
            <ol type="a">
                <li> Je kan een tijd per vraag instellen </li>
                <li> Het aantal vragen kan gekozen worden </li>
            </ol>
            <li> Totaaltijd Dat je bezig bent wordt bijgehouden </li>
            <li> Een keuze maken over welke vraag als volgende in de ondervraging komt op basis van vorige prestaties, de voormalig fout ingevulde vragen worden herhaald.</li>
        </ol>
    <li>Oefenexamens:</li>
        <ol  type="i">
            <li> Je kan een proefexamen maken op basis van geselecteerde vragen.</li>
            <li> Je kan deze customizen </li>
                <ol type="a">
                    <li> Het aantal vragen </li>
                    <li> Je kan een maximum totaaltijd instellen </li>
                </ol>
            <li> Neemt een subset van geselecteerde vragen zonder rekening te houden met prestaties </li>
        </ol>
    <li> Accounts: </li>
        <ol type="i"> 
            <li> Kan accounts aanmaken en beheren </li>
                <ol type="a">
                    <li> Docent account </li>
                    <li> Student account </li>
                </ol>
        </ol>
    <li> Groepen: </li>
        <ol type="i">
            <li> Een docent kan een groep aanmaken </li>
            <li> Kan vragen delen in de groep </li>
            <li> De docent ontvangt info over hoe de studenten de ondervragingen gemaakt hebben </li>
        </ol>
    <li> Opslaan van vragen: </li>
        <ol type="i">
            <li> Lokaal opslaan </li>
            <li> Indien de gebruiker ingelogd is, dan kan de gebruiker online een back-up hiervan op de server opslaan </li>
        </ol>
</ol>

### Usability
<ol>
    <li>Kaart metafoor gebruikt om flash cards voor te stellen. Het metafoor zorgt ervoor dat de flashcards intuïtief en herkenbaar zijn voor gebruikers, zodat ze gemakkelijk begrijpen hoe ze de tool moeten gebruiken. </li>
    <li>Tooltips die mogelijke keuzes toelichten bij het maken van de flashcards. Dit kan mogelijks ook nog op andere plaatsen gehanteerd worden indien nodig. </li>
    <li>Diverse customizability(Kleuren, lettertypes) </li>
    <li>Consistente interface en terminologie, dit zorgt ervoor dat de gebruikers niet voor mogelijke verrassingen komen te staan als sommige elementen andere functionaliteiten hebben dan ze gedacht hadden. </li>
    <li>Interface moet aangenaam over komen en gebruikt worden voor lange tijd </li>
</ol>

### Reliability
<ol>
    <li>Lage faalfrequentie van de server: 99% server uptime</li>
    <li>Er is een offline modus, indien de connectie verloren gaat</li>
    <li>Online opgeslagen vragen of examens kunnen niet geraadpleegd worden</li>
    <li>Nieuw gemaakte vragen kunnen enkel offline opgeslagen worden</li>
    <li>Implementeer correcte foutafhandeling (zie hieronder) voor de gebruiker te verwittigen dat ze offline zijn</li>
    <li>Foutafhandeling: Implementeer duidelijke foutmeldingen die gebruikers helpen begrijpen wat er mis is gegaan en hoe ze het probleem kunnen oplossen.</li>
    <li>Herstelopties: Bied opties om per ongeluk verwijderde flashcards of groepen van flashcards te herstellen.</li>
    <li>Data-integriteit: Garandeer dat gegevens (flashcards) consistent en correct worden opgeslagen</li>
</ol>

### Performance
<ol><li>Responsieve interface met een maximum response tijd van 50ms</li></ol>


### Supportability
<ol>
    <li>Gebruikers kunnen ondervragingen zelf configureren (Aantal vragen, tijdslimiet, …)</li>
    <li>Consistente code en contracten (er is een vaste structuur) om onderhoudbaarheid te verhogen</li>
</ol>

### +
#### Ontwerp requirements
<ol><li>Moet compatibel zijn met Windows en Linux systemen</li></ol>

#### Implementatie requirements
<ol><li>Moet OOP geprogrammeerd worden</li></ol>

#### Interface requirements
<ol><li>Authenticatie voor met server te communiceren</li></ol>

#### Fysieke requirements (hardware)
<ol><li>Computer(Windows of Linux OS)</li></ol>

