
import shutil
import os
import fnmatch

_file_exists = os.path.isfile
_dir_exists = os.path.isdir
_join = os.path.join

BtDistDir = "cocos2d-x-dist"

def _copy(config, src):
    if not _file_exists(src):
        print("file not found, ignored. (src='{}')".format(src))
        return

    dest = _join(BtDistDir, config, os.path.basename(src))
    if not _dir_exists(os.path.dirname(dest)):
        os.makedirs(os.path.dirname(dest))

    shutil.copyfile(src, dest)
    print("file copied. (dest='{}')".format(dest))


def _copy_3rd(config, pkgpath, libname):
    _copy(config, r"cocos2d-x\external\{}\prebuilt\{}.lib".format(pkgpath, libname))
    _copy(config, r"cocos2d-x\external\{}\prebuilt\{}.dll".format(pkgpath, libname))


def copy_files(config):
    _copy(config, r"cocos2d-x\build\win32-msvc-vs2013-x86\lib\{}\cocos2d.lib".format(config))
    _copy(config, r"cocos2d-x\build\win32-msvc-vs2013-x86\cocos\{}\cocos2d.dll".format(config))
    _copy_3rd(config, r"win32-specific\gles", "glew32")
    _copy_3rd(config, r"win32-specific\OpenalSoft", "OpenAL32")
    _copy_3rd(config, r"win32-specific\MP3Decoder", "libmpg123")
    _copy_3rd(config, r"win32-specific\OggDecoder", "libogg")
    _copy_3rd(config, r"win32-specific\OggDecoder", "libvorbis")
    _copy_3rd(config, r"win32-specific\OggDecoder", "libvorbisfile")
    _copy_3rd(config, r"websockets", r"win32\websockets")


def main():
    print("erasing existing dist directory...")
    if os.path.exists(BtDistDir):
        shutil.rmtree(BtDistDir)

    # different configurations go to different folder, since cocos2d doesn't postfix '_d' for debug build 
    copy_files("Debug")
    copy_files("Release")
    
    print("done successfully!")

if __name__ == '__main__':
    main()
