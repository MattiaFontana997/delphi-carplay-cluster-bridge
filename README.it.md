# Delphi CarPlay Cluster Bridge

Progetto aperto di ricerca per portare indicazioni CarPlay e, sui quadri compatibili, mappe CarPlay sulle unità MIB2 Delphi. Primo firmware studiato offline: `MST2_EU_VW_P0879D`.

**Non è ancora una patch funzionante.** Questa versione contiene documentazione e un analizzatore offline. Non include installer, modifiche alla radio, decodifica RGI o un ponte video operativo.

## Due obiettivi distinti

1. Frecce, distanze e altre indicazioni disponibili su quadri analogici o VC che le supportano.
2. Mappa CarPlay su VC con capacità video verificata.

Supporto indicazioni e supporto mappe vanno verificati separatamente per ciascun quadro. MOST e presenza di uno schermo digitale non bastano a dimostrarli.

## NavIgnore / NavActiveIgnore

Per il percorso progettato che mantiene attiva la navigazione nativa insieme a CarPlay, prevediamo come prerequisito **una patch NavIgnore compatibile con il firmware Delphi, oppure una gestione equivalente dell'esclusione tra le due navigazioni**. La necessità della patch specifica e la sua convivenza con il futuro bridge RGI/video sono ancora da validare: non è un requisito universale già dimostrato. L'analizzatore offline non richiede alcuna patch sulla radio.

NavIgnore da sola non trasferisce indicazioni RGI o mappe CarPlay al quadro.

**Crediti:** il contributo originale NavActiveIgnore nel progetto MIB2 High è di **Andrew Leech (`andrewleech` / `alelec`)**, documentato nella [PR originale #189](https://github.com/jilleb/mib2-toolbox/pull/189) e nel [progetto mib2-lsd-patching](https://gitlab.com/alelec/mib2-lsd-patching). Le segnalazioni Delphi citano l'ambiente **Congo & Duke**, ma l'autore dell'esatto adattamento installato va confermato: non attribuiamo automaticamente il port Delphi all'autore originale.

Vedi [requisiti, compatibilità e fonti](docs/navignore.md). Questa versione non distribuisce né installa la patch.

## Primo strumento

Con Node.js 22 o successivo, senza dipendenze:

```sh
node --test
node tools/analyze-firmware.cjs /percorso/EXTBIN_APPS.bin local/rapporto.json
```

Creare prima la cartella di destinazione. Il rapporto non deve esistere già. Il programma legge GZIP/TAR, controlla le intestazioni TAR, calcola hash e cerca indizi nei componenti selezionati senza estrarli o eseguirli. I limiti e i formati supportati sono descritti nel [README inglese](README.md).

Nel P0879D abbiamo osservato servizi CarPlay, Cluster e MOST e componenti video H.264. Sono evidenze statiche: non dimostrano che RGI o un secondo schermo CarPlay siano già disponibili.

## Collaborazione

Cerchiamo contributi su QNX/ARM, CarPlay, HMI/DSI Delphi, guida del cluster e video MOST. Vedi [attività iniziali](docs/roadmap.md), [architettura](docs/architecture.md) e [regole per contribuire](CONTRIBUTING.md).

Condividere codice originale, documentazione e campioni sintetici. Mantenere locali firmware, librerie proprietarie, chiavi, VIN e log grezzi. Nessuna installazione sulla radio è prevista in questa fase.
