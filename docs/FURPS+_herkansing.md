### Functionality
<ol>
    <li> Vakken: </li>
        <ol type="i">
            <li> Een docent kan een vak aanmaken en beheren </li>
                <ol type="a">
                    <li>Andere docenten toevoegen en verwijderen </li>
                    <li>Een titel toewijzen per docent per vak, bv. assistent</li>
                </ol>
            <li> Kan proefexamens delen in het vak en beheren</li>
                <ol type="a">
                    <li>Verwijderen en aanpassen </li>
                    <li>Gemiddelde scores zien</li>
                </ol>
        </ol>
    <li> Officiele proefexamens: </li>
        <ol type="i">
            <li> De studenten zien duidelijk welke proefexamens officieel zijn </li>
            <li>De student kan het proefexamen maken en als hij online is wordt zijn score online opgeslagen</li>
            <li>De student kan het proefexamen niet aanpassen</li>
            <li> De docent ontvangt info over hoe de studenten de proefexamen gemaakt hebben </li>
        </ol>
</ol>

### Usability
<ol>
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
    <li>Data-integriteit: Garandeer dat gegevens (proefexamens, scores) consistent en correct worden opgeslagen</li>
</ol>

### Performance
<ol><li>Responsieve interface met een maximum response tijd van 50ms</li></ol>


### Supportability
<ol>
    <li>Enkel docenten kunnen officiele proefexamens configureren (Aantal vragen, tijdslimiet, …), de studenten die deze maken kunnen dit niet</li>
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

