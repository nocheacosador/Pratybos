#!/usr/bin/env python3

import os
import sys
import curses
from curses import wrapper

config = {
    'build_dir':     '',
    'build_file':    'main.c',
    'build_out':     'main',
    'build_command': 'gcc ${build_file} -o ${build_out}',
    'run_command':   '\'${build_dir}/${build_out}\''
}

def FillInConfigVariables(string: str):
    global config

    for name in config:
        string = string.replace('${' + name + '}', config[name])

    return string


def ShowDirectorySelectionMain(stdscr):
    global config

    curses.curs_set(False)
    stdscr.clear()

    dir_list = os.listdir(os.getcwd())
    dir_list = [x for x in dir_list if os.path.isdir(x) and x.split('/')[-1] != '.vscode']

    win_title = 'Choose Project'
    win_height, win_width = 0, 0
    win_height = len(dir_list)

    for directory in dir_list:
        directory = directory.split('/')[-1]
        win_width = max(win_width, len(directory))
    win_width = max(win_width, len(win_title))

    win_height = win_height + 3
    win_width  = win_width  + 8
    win_y = int((curses.LINES - win_height) / 2)
    win_x = int((curses.COLS  - win_width)  / 2)

    win = stdscr.subwin(win_height, win_width, win_y, win_x)

    win.border()

    win.addstr(0, int((win_width - len(win_title)) / 2), win_title, curses.A_REVERSE)

    selection_index = 0
    
    y = 2
    for directory in dir_list:
        if dir_list[selection_index] == directory:
            win.addch(y, 3, '>', curses.A_BLINK + curses.A_BOLD)
            win.addstr(y, 4, directory, curses.A_BOLD)
        else:
            win.addstr(y, 4, directory)
        y += 1

    stdscr.refresh()

    while True:
        key = stdscr.getch()

        if key == curses.KEY_ENTER or key == 10 or key == 13:
            config['build_dir'] = os.getcwd() + '/' + dir_list[selection_index]
            break
        elif key == curses.KEY_DOWN:
            selection_index = min(selection_index + 1, len(dir_list) - 1)
        elif key == curses.KEY_UP:
            selection_index = max(selection_index - 1, 0)

        y = 2
        for directory in dir_list:
            if dir_list[selection_index] == directory:
                win.addch(y, 3, '>', curses.A_BLINK + curses.A_BOLD)
                win.addstr(y, 4, directory, curses.A_BOLD)
            else:
                win.addstr(y, 3, ' ' + directory)
            y += 1

        win.refresh()


def ShowDirectorySelection():
    wrapper(ShowDirectorySelectionMain)


def Build():
    global config

    print('[Build] Build directory \'{}\''.format(config['build_dir']))
    print('[Build] Entering build directory...')
    os.chdir(config['build_dir'])

    if os.path.isfile(config['build_file']):
        print('[Build] Build file found!')
    else:
        print('[Build] Build failed. Build file not found.')
        return False

    if os.path.isfile(config['build_out']):
        os.remove(config['build_out'])

    cmd = FillInConfigVariables(config['build_command'])
    print(cmd)
    os.system(cmd)

    if os.path.isfile(config['build_out']):
        print('[Build] Build succeeded.')
        return True
    else:
        print('[Build] Build failed.')
        return False


def Run():
    global config

    print('[Run] Running...')

    cmd = FillInConfigVariables(config['run_command'])
    print(cmd)
    os.system(cmd)


def Main():
    ShowDirectorySelection()
    
    build_ok = Build()
    
    if build_ok and '--run' in sys.argv:
        Run()

if __name__ == '__main__':
    Main()