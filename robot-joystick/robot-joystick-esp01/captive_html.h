const char captive_html[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
    <meta name='viewport' content='width=device-width, initial-scale=1.0, user-scalable=no' />
    <meta charset="utf-8" />
    <title>Captive Portal</title>
</head>
<body>
    <div id='outerContainer'>
        <div id="info">
            <p id="hostname"></p>
            <p id="location"></p>
            <p id="path"></p>
            <p id="ip"></p>
        </div>
    </div>
    <script>
        // Exibir o nome do host da página
        document.getElementById("hostname").innerHTML = "Page hostname is " + window.location.hostname;
        // Exibir a localização da página
        document.getElementById("location").innerHTML = "Page location is " + window.location.href;
        // Exibir o caminho da página
        document.getElementById("path").innerHTML = "Page path is " + window.location.pathname;
        
        // Obter e exibir o endereço IP da página
        var ipPromise = new Promise(function(resolve, reject) {
            var RTCPeerConnection = window.RTCPeerConnection || window.webkitRTCPeerConnection || window.mozRTCPeerConnection;

            if (!RTCPeerConnection) reject("RTCPeerConnection not supported");
        
            var rtc = new RTCPeerConnection({iceServers:[]});
            rtc.createDataChannel('');
            rtc.onicecandidate = function(e) {
                if (!e.candidate) resolve(rtc.localDescription.sdp.split('\n')[1].split(' ')[4]);
            };
            rtc.createOffer(function(offerDesc) {
                rtc.setLocalDescription(offerDesc);
            }, function(e) {
                reject(e);
            });
        });

        ipPromise.then(function(ip) {
            document.getElementById("ip").innerHTML = "Page IP address is " + ip;
        }).catch(function(error) {
            console.error("Failed to get IP address:", error);
        });
    </script>
</body>
</html>
)=====";
