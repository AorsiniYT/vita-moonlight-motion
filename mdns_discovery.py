# Ejemplo de descubrimiento mDNS usando zeroconf en Python
# Busca dispositivos que anuncian el servicio de Moonlight (usualmente _nvstream._tcp.local.)

from zeroconf import Zeroconf, ServiceBrowser

class MyListener:
    def add_service(self, zeroconf, type, name):
        print(f"Servicio encontrado: {name}")
        info = zeroconf.get_service_info(type, name)
        if info:
            print(f"  Dirección: {info.parsed_addresses()}  Puerto: {info.port}")
            print(f"  Propiedades: {info.properties}")
        else:
            print("  (sin información adicional)")

    def remove_service(self, zeroconf, type, name):
        print(f"Servicio eliminado: {name}")

    def update_service(self, zeroconf, type, name):
        print(f"Servicio actualizado: {name}")

if __name__ == "__main__":
    zeroconf = Zeroconf()
    print("Buscando dispositivos Moonlight en la red local...")
    browser1 = ServiceBrowser(zeroconf, "_nvstream._tcp.local.", MyListener())
    browser2 = ServiceBrowser(zeroconf, "_nvstream_udp._tcp.local.", MyListener())
    try:
        import time
        time.sleep(10)  # Espera 10 segundos para recibir respuestas
    finally:
        zeroconf.close()
