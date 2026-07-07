import os
import ctypes
import sip
from qgis.PyQt.QtCore import QLibrary

_cpp_plugin_lib = None

def classFactory(iface):
    global _cpp_plugin_lib
    
    plugin_dir = os.path.dirname(__file__)
    dll_path = os.path.join(plugin_dir, "cajander.dll")
    if _cpp_plugin_lib is None:
        _cpp_plugin_lib = QLibrary(dll_path)
        if not _cpp_plugin_lib.load():
            raise RuntimeError(f"CAJANDER C++ Link Error: {_cpp_plugin_lib.errorString()}")
    try:
        native_dll = ctypes.CDLL(dll_path)
        if hasattr(native_dll, 'initPlugin'):
            native_dll.initPlugin.argtypes = [ctypes.c_void_p]
    except Exception as e:
        raise RuntimeError(f"Failed to bind C++ methods: {e}")

    class CppBridgePlugin:
        def __init__(self, iface):
            self.iface = iface
            
        def initGui(self):
            if hasattr(native_dll, 'initPlugin'):
                iface_void_ptr = sip.unwrapinstance(self.iface)
                native_dll.initPlugin(iface_void_ptr)
                
        def unload(self):
            if hasattr(native_dll, 'unloadPlugin'):
                native_dll.unloadPlugin()
                
    return CppBridgePlugin(iface)
