set number
set incsearch
set hlsearch
syn on
set smartindent
set autoindent
color koehler
filetype plugin indent on
set grepprg=grep\ -nH\ $*
let g:tex_flavor = "latex"
set runtimepath=~/.vim/plugins,~/.vim,$VIM/vimfiles,$VIMRUNTIME,$VIM/vimfiles/after,~/.vim/after
set lcs=tab:>-,eol:$
set list
