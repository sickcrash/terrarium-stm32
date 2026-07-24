basta solo:

npm start

si avviano il venv, il demone python e il server node con log concorrenti

questo perchè abbiamo messo in package.json: 

"concurrently -k -p \"[{name}]\" -n \"PYTHON,NODE\" -c \"yellow,cyan\" \"venv/bin/python gateway_daemon.py\" \"node server.js\""
(VALIDO PER MACOS)

per windows: 
"concurrently -k -p \"[{name}]\" -n \"PYTHON,NODE\" -c \"yellow,cyan\" \"venv\\Scripts\\python.exe gateway_daemon.py\" \"node server.js\""