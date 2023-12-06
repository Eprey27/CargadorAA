**⚠️ WARNING: Version 0.0.0-alpha ⚠️**

This software version **has not been tested in a real Arduino environment**. It is strongly recommended **not to use it if you are not familiar to handling battery power supply routines**. Improper handling of batteries and electronic circuits can lead to significant risks, including but not limited to, short circuits, overheating, component damage, or even fire hazards.

**Use this software at your own risk and only if you understand and are capable of safely managing the risks associated with electronic systems and batteries.**

---

**⚠️ ADVERTENCIA: Versión 0.0.0-alpha ⚠️**

Esta versión del software **no ha sido probada en un entorno real con Arduino**. Se recomienda encarecidamente **no utilizarla si no tienes experiencia previa con rutinas de suministro de energía con baterías**. El manejo inadecuado de baterías y circuitos electrónicos puede conllevar riesgos significativos, incluyendo, pero no limitado a, cortocircuitos, sobrecalentamiento, daños en los componentes, o incluso riesgos de incendio.

**Utiliza este software bajo tu propia responsabilidad y solo si comprendes y estás capacitado para manejar de manera segura los riesgos asociados con sistemas electrónicos y baterías.**

# Cargador de Baterías AA con Arduino

Para implementar este proyecto de cargador de baterías con Arduino, proporcionaremos una guía paso a paso para construir el circuito y montar los componentes necesarios. Esta guía asume que tienes conocimientos básicos de electrónica y soldadura. Vamos a dividir el proceso en etapas claras.

## Materiales Necesarios

1. Arduino Uno
2. Protoboard
3. Cables de conexión (jumpers)
4. Resistencias
5. LEDs
6. Sensores de temperatura (como LM35 o similares)
7. Pantalla LCD con interfaz I2C (como LiquidCrystal_I2C)
8. Portapilas AA o AAA
9. Módulos de carga de batería (si son recargables)
10. Interruptor o botón
11. Diodos (opcional, para protección)
12. Ventiladores pequeños (2x)
13. Transistores para controlar los ventiladores (como 2N2222 o similares)
14. Fuente de alimentación para Arduino

## Pasos para Construir el Circuito

### Paso 1: Preparación de la Placa Arduino

1. **Conectar Arduino a la Protoboard**:
   - Coloca tu Arduino Uno en un extremo de la protoboard para que tengas espacio para los demás componentes.

### Paso 2: Configuración de los Sensores de Temperatura

1. **Montaje de Sensores de Temperatura**:
   - Conecta los sensores de temperatura en la protoboard. Asegúrate de que cada sensor tenga acceso a una línea de alimentación (5V) y tierra (GND).
   - Conecta la salida de cada sensor a los pines analógicos del Arduino (A0 para la placa y A1 para las baterías).

### Paso 3: Configuración de la Pantalla LCD I2C

1. **Conexión del LCD**:
   - Conecta el VCC del LCD a 5V en Arduino, GND a GND, SDA a A4 y SCL a A5.

### Paso 4: Configuración de los Ventiladores y su Control

1. **Montaje de Ventiladores**:
   - Coloca los ventiladores de forma que uno enfríe el Arduino y el otro las baterías.
2. **Control de Ventiladores**:
   - Usa transistores para controlar los ventiladores. Conecta el colector al ventilador, el emisor a GND y la base a través de una resistencia a los pines digitales del Arduino (2 y 3).

### Paso 5: Configuración de las Baterías y LEDs

1. **Montaje de Portapilas**:
   - Coloca los portapilas en la protoboard o cerca de ella.
2. **Conexión de los Portapilas**:
   - Conecta los portapilas a los pines analógicos del Arduino (A2 a A5) para leer el voltaje. Podrías necesitar resistencias si decides hacer un divisor de voltaje.
3. **Instalación de LEDs**:
   - Conecta un LED para cada batería a la protoboard, con una resistencia adecuada en serie, y luego a los pines digitales del Arduino (8 a 11).

### Paso 6: Configuración del Interruptor

1. **Montaje del Interruptor**:
   - Coloca el interruptor en la protoboard y conéctalo a un pin digital del Arduino (pin 12) y a GND. Puede ser necesario una resistencia pull-up o pull-down según tu configuración.

### Paso 7: Conexión de la Fuente de Alimentación y Pruebas

1. **Conectar Arduino a la Fuente de Alimentación**:
   - Asegúrate de que el Arduino esté desconectado de tu computadora o de cualquier fuente de energía antes de conectar la fuente de alimentación externa.

2. **Pruebas**:
   - Realiza pruebas iniciales para asegurarte de que cada componente esté correctamente conectado y funcione según lo esperado.

## Recomendaciones Finales

- **Verificación**: Antes de conectar la energía, revisa dos veces todas las conexiones para evitar cortocircuitos.
- **Seguridad**: Ten precaución al trabajar con componentes electrónicos, especialmente alrededor de la energía y las baterías.
- **Documentación**: Mantén un registro o un esquema de tus conexiones
