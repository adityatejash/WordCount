# WordCount — Text Analyzer

A full-stack text analysis tool with a C++ HTTP backend and a pure HTML/CSS/JS frontend.

## Features

| Feature | Details |
|---|---|
| **Basic stats** | Words, characters, sentences, spaces, lines |
| **File upload** | Drag & drop or click — `.txt`, `.md`, `.csv` and more |
| **Dark mode** | Toggle persists via localStorage |
| **Export report** | Download `.txt` or copy to clipboard |
| **Duplicate detector** | Finds repeated sentences, highlights them in red |
| **Readability score** | Flesch Reading Ease (0–100) with grade label |
| **Top words** | Frequency analysis excluding stopwords |
| **Reading time** | Estimated at 238 wpm |

## Project structure

```
├── WordCount.cpp    # C++ backend — HTTP server + all analysis logic
├── index.html       # Frontend — served by the C++ server
├── render.yaml      # Render deployment config
└── README.md
```

## Deploy to Render

1. Push this repo to GitHub
2. Go to [render.com](https://render.com) → New → Web Service
3. Connect your GitHub repo
4. Render auto-detects `render.yaml` and uses:
   - **Build:** `g++ -std=c++17 -O2 -o wordcount WordCount.cpp`
   - **Start:** `./wordcount`
5. Done — your app is live!

## Run locally

```bash
g++ -std=c++17 -O2 -o wordcount WordCount.cpp
./wordcount
# Open http://localhost:8080
```

## API

The C++ server exposes two endpoints:

| Method | Path | Description |
|---|---|---|
| `GET` | `/` | Serves `index.html` |
| `POST` | `/analyze` | Accepts plain text body, returns JSON analysis |

### Example API call

```bash
curl -X POST http://localhost:8080/analyze \
  -H "Content-Type: text/plain" \
  -d "Hello world. Hello world. This is a test sentence."
```

### Response shape

```json
{
  "words": 10,
  "characters": 51,
  "sentences": 3,
  "spaces": 9,
  "lines": 0,
  "readability": { "score": 83, "grade": "Easy" },
  "duplicates": {
    "count": 1,
    "unique": 2,
    "sentences": ["Hello world."]
  },
  "topWords": [
    { "word": "hello", "count": 2 },
    { "word": "world", "count": 2 }
  ],
  "report": "═══ ... report text ..."
}
```
