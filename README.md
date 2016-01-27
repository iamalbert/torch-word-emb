#torch-word-emb
Load your word embeddings to Torch tensor.


## Install
Torch 7 required.
```
luarocks install torch-word-emb
```
or
```
git clone https://github.com/iamalbert/torch-word-emb
luarocks install torch-word-emb/torch-word-emb-1.2-0.rockspec
```

## Usage

```lua
local wordemb = require 'wordemb'
```

### wordemb.load_word2vec_bin(path)
read word2vec binary-format model from `path`.

returns a table of two fields.
  - `vec` is a torch FloatTensor of size `V x D`, where `V` is the vocabulary size and `D` is the dimension of word2vec.
  - `words` is a table of (word,offset) pairs, repesenting the corresponding position of such word in `vec`.

```lua
local w2v = wordemb.load_word2vec_bin("/path/to/word2vec/model.bin")
print(w2v.vec:size())
print(w2v.vec[ w2v.words["apple"] ] )
```

### wordemb.load_word2vec_text(path)
read word2vec text-format model from `path`.

### wordemb.load_glove_text(path)
read GloVe text-format model from `path`.
