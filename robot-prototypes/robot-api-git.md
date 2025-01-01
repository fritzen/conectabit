# ESP32-CAM Robot - API Documentation

## Base URL
```
http://{IP-DO-ESP32}
```
*Substitua `{IP-DO-ESP32}` pelo IP mostrado no Serial Monitor ao iniciar.*

## Endpoints

### 1. Video Streaming

Retorna stream de vídeo em tempo real da câmera.

**Endpoint**: `/stream`

* **URL**: `http://{IP-DO-ESP32}:81/stream`
* **Método**: GET
* **Porta**: 81
* **Response**: MJPEG Stream
* **Content-Type**: multipart/x-mixed-replace

**Exemplo de uso**:
```
http://192.168.1.216:81/stream
```

### 2. Controle dos Motores

Controla a direção e velocidade dos motores.

**Endpoint**: `/control`

* **URL**: `http://{IP-DO-ESP32}/control`
* **Método**: POST
* **Porta**: 80
* **Headers**: 
  * Content-Type: application/json
* **Body Parameters**:
```json
{
  "motor1": integer,  // Valores: -255 a 255
  "motor2": integer   // Valores: -255 a 255
}
```
* **Response**: 
```json
{
  "status": "ok"
}
```

**Valores dos motores**:
* Positivo (1 a 255): Motor gira para frente
* Negativo (-255 a -1): Motor gira para trás
* Zero (0): Motor para

## Exemplos de Uso

### Comandos Básicos

1. **Mover para frente**
```json
{
    "motor1": 200,
    "motor2": 200
}
```

2. **Mover para trás**
```json
{
    "motor1": -200,
    "motor2": -200
}
```

3. **Virar à direita**
```json
{
    "motor1": 200,
    "motor2": -200
}
```

4. **Virar à esquerda**
```json
{
    "motor1": -200,
    "motor2": 200
}
```

5. **Parar**
```json
{
    "motor1": 0,
    "motor2": 0
}
```

### Exemplos com cURL

```bash
# Mover para frente
curl -X POST http://192.168.1.216/control \
  -H "Content-Type: application/json" \
  -d '{"motor1": 200, "motor2": 200}'

# Mover para trás
curl -X POST http://192.168.1.216/control \
  -H "Content-Type: application/json" \
  -d '{"motor1": -200, "motor2": -200}'

# Parar
curl -X POST http://192.168.1.216/control \
  -H "Content-Type: application/json" \
  -d '{"motor1": 0, "motor2": 0}'
```

## Notas Importantes

1. O streaming e controle operam em portas separadas para melhor performance
2. Os valores dos motores devem estar entre -255 e 255
3. Recomenda-se enviar valores absolutos mínimos de 100 para garantir movimento dos motores
