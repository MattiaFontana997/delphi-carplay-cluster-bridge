# Delphi CarPlay Cluster Bridge

Progetto aperto di ricerca per portare indicazioni CarPlay e, sui quadri compatibili, mappe CarPlay sulle unità MIB2 Delphi. Primo firmware studiato offline: `MST2_EU_VW_P0879D`.

**Non è ancora una patch funzionante sulla radio.** Include documentazione, analizzatore offline e un nucleo C99 per decodifica RGI e preparazione Identify. Non include installer, ricevitore Delphi operativo o un ponte video.

## Versione 0.1.0 sperimentale

Primo rilascio del [codice C e dei test](native/rgi/README.md): decoder, gestione delle sessioni e preparazione in memoria del componente Identify RGI. Il collegamento al mittente originale Delphi e l'accettazione dell'iPhone restano da verificare. Le capacità dichiarate nei test sono sintetiche, non capacità hardware certificate.

## Aggiornamento 0.0.4

Ricostruiti offline i builder Identify, la gestione destinazioni e il dispatcher controllo iAP2. Nel ramo ricostruito i messaggi RGI in ingresso non hanno un gestore dedicato. Trovati anche l'estensione delle liste dei messaggi dichiarati e il collegamento client/driver per l'invio grezzo: sono candidati di sviluppo, non un ricevitore già operativo.

[Rapporto iAP2](docs/research/2026-10-30-iap2.it.md) · [Evidenze iAP2](docs/research/evidenze_iap2_delphi.json)

## Risultati della 0.0.3

La nuova versione integra i risultati dell'analisi statica: individuato un candidato per l'uscita delle indicazioni HMI/BAP, esaminate le interfacce Java CarPlay e rilevato un limite nella gestione dello stream video aggiuntivo. Include anche il confronto dei componenti rilevanti con P0891D.

L'ingresso reale degli eventi RGI resta da identificare. **Non è ancora una versione installabile sulla radio**; il codice dell'analizzatore offline è invariato.

[Rapporto completo](docs/research/2026-10-30-delphi-carplay.it.md) · [Evidenze tecniche](docs/research/evidenze_carplay_delphi.json) · [Novità delle versioni](CHANGELOG.md)

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
