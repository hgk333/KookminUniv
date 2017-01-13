syntax on
set t_Co=256
set number
set hlsearch
set ruler
set ignorecase
set showcmd
set wildmenu
set autoindent
set cindent
set laststatus=2
set tabstop=4
set ai
set si
set showmatch
set fileencodings=utf-8,euc-kr
set shiftwidth=4
set expandtab
set backspace=indent,eol,start
set nowrap
set nobackup
set showcmd
set paste

set nocompatible

set nocompatible
filetype off
set rtp+=~/.vim/bundle/Vundle.vim
call vundle#begin()
    Plugin 'gmarik/Vundle.vim'
    Plugin 'tpope/vim-fugitive'
    Plugin 'L9'
    Plugin 'git://git.wincent.com/command-t.git'
    Plugin 'file:///home/gmarik/path/to/plugin'
    Plugin 'rstacruz/sparkup', {'rtp': 'vim/'}
    Plugin 'user/L9', {'name': 'newL9'}
call vundle#end()
filetype plugin indent on
