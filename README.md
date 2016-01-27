# torch-word-emb
load word embeddings to Torch tensor.


## Install
Torch 7 required.
```
git clone https://github.com/iamalbert/torch-word-emb
luarocks install torch-word-emb/torch-word-emb-1.1-4.rockspec
```


## Usage

```lua
local wordemb = require 'wordemb'

local w2v = wordemb.load_word2vec_bin("/path/to/word2vec/model.bin")
```

### wordemb.load_word2vec_bin(path)
read word2vec binary model from `path`.

returns a table of two fields.
  - `vec` is a torch FloatTensor of size `V x D`, where `V` is the vocabulary size and `D` is the dimension of word2vec.
  - `words` is a table of (word,offset) pairs, repesenting the corresponding position of such word in `vec`.

```lua
print(w2v.vec:size())
print(w2v.vec[ w2v.words["apple"] ] )
```


