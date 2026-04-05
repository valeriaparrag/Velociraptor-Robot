'''*******************************************************
 *   CONTROL DE VELOCIRAPTOR CON ESP32 POR WIFI -- PROYECTO ROS
 *   - Recibe comandos tipo: LETRA:NUM (A,R,I,D : 0–255)
 *   - Controla motores mediante puente H
 *   - Suscriptor al tópico wifi_transmitter
 *   - Autores: Valeria Andrea Parra García - María Alejandra Díaz Ortiz
 ********************************************************'''

import rclpy
from std_msgs.msg import String
import socket

'''********************************************************
 *  CONEXION WIFI CON LA ESP
 ********************************************************'''

def connect_to_esp32():
    global esp32
    # Create a TCP/IP socket
    esp32 = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    # Attempt to connect to the specified ESP32 IP and port
    esp32.connect(("192.168.121.219", 8080))

'''********************************************************
 *  FUNCIÓN DE CALLBACK
 ********************************************************'''

def message_callback(msg):
    global esp32, node

    data = msg.data.strip() #Recibe el dato y quita caraacteres finales
    print(f'Recibido del publicador: {data}')

    # Validación básica formato letra:número
    if ":" not in data:
        node.get_logger().error("Formato inválido. Use LETRA:NUMERO, por ejemplo A:15")
        return
    # Separa la letra del número
    letra, numero = data.split(":", 1)

    # Limpieza de letra y número
    letra = letra.strip()
    numero = numero.strip()

    # Validación para letra y número
    if not letra.isalpha():
        node.get_logger().error("La primera parte debe ser una letra")
        return
    if not numero.isdigit():
        node.get_logger().error("La segunda parte debe ser un número")
        return
    
    # Envío a la ESP en formato letra:numero
    serial_msg = f"{letra}:{numero}\n"
    print(f'Enviando a ESP: {serial_msg}')
    esp32.send(msg.data.encode('utf-8'))

'''********************************************************
 *  CREACION DEL NOTO Y TOPICO
 ********************************************************'''

def main():
    global esp32, node
    rclpy.init()
    # 1 Crear el nodo
    node = rclpy.create_node('wifi_transmitter')
    # 2 Crear el suscriptor (tipo dato, nombre topico, callback, mensaje)
    node.create_subscription(String, 'wifi_transmitter', message_callback, 10)
    connect_to_esp32()
    #3 Para no matar el nodo
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
