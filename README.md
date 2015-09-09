# Interaktivni-dokazivac
Interkativni dokazivac u prirodnoj dedukciji (Qt)

Ovaj interaktivni dokazivač zasniva se na prirodnoj dedukciji. U prirodnoj dedukciji koriste se logički veznici:
¬, ∧, ∨, ⇒, kao i logička konstanta ⊥. Za svaki logički veznik postoje pravila koja ga uvode (pravila I-tipa) i pravila koja ga eliminišu (pravila E-tipa). Postoji sistem prirodne dedukcije za klasičnu logiku i sistem prirodne dedukcije za intuicionističku logiku. U sistemu prirodne dedukcije za klasičnu logiku postoji jedna aksiomska shema: A ∨ ¬A. Sistem za intuicionističku logiku nema aksioma. Ovo je verzija dokazivača u sistemu intuicionističke logike.
Dokaz (dedukcija, izvod) je stablo čijem je svakom čvoru pridružena formula, a svakom listu je pridružena pretpostavka. Dokaz se prikazuje u vidu stabla čiji su listovi na vrhu, a koren na dnu.

Uputstvo za korišćenje:

Na samom početku postoje dve opcije: možete ukucati formulu koju želite da dokazujete, ili učitati formulu iz fajla.
Nakon toga treba potvrditi da želite da započnete dokaz klikom na dugme 'potvrdi'. Tada se ispisuje formula koja se dokazuje. Formulu treba obeležiti klikom na nju (tada formula postaje crvena), i nakon toga primeniti neko od pravila. Neophodno je uvek obeležiti formulu nad kojom se pravilo primenjuje, pa tek nakon toga primeniti pravilo.
U svakom koraku eliminišu se pravila koja se ne mogu primeniti u tom slučaju, a od preostalih treba izabrati jedno. Nakon primene, rezultat se dodaje u stablo i ispisuje se koje ste pravilo primenili. U svakom trenutku postoji mogućnost za poništavanje prethodno primenjenih pravila, pritiskom na dugme 'poništi', i tada se odgovarajući čvorovi u stablu eliminišu. Pravila se ne moraju poništavati u redosledu kojim su primenjivana. U slučaju da prilikom primene nekog pravila dođe do preklapanja čvorova(pri ispisu), postoji mogućnost za pomeranje čvora na odgovarajuću poziciju, radi bolje preglednosti.
U slucaju da list pripada skupu pretpostavki bice oznacen uglastim zagradama ( '[]' ). Dokaz se završava kada svi listovi pripadaju skupu pretpostavki.
