from bluepy import btle

print("Connecting...")
dev = btle.Peripheral("30:AE:A4:1B:6F:BE")

print("Services...")
for svc in dev.services:
	print(str(svc))

srv = btle.UUID("6E400001-B5A3-F393-E0A9-E50E24DCCA9E")

tts = dev.getServiceByUUID(srv)
for ch in tts.getCharacteristics():
	print(str(ch))
	print(ch.read())

dev.disconnect()
