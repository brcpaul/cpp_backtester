import pandas as pd
import time
import random
import string
import os

# Paramètres
script_dir = os.path.dirname(os.path.abspath(__file__))
output_file = os.path.join(script_dir, 'orders.csv')
instruments = ['AAPL', 'EURUSD', 'BTCUSD', 'TSLA']
sides = ['BUY', 'SELL']
types = ['LIMIT', 'MARKET']
actions = ['NEW', 'MODIFY', 'CANCEL']

# Générateur d'horodatage en nanosecondes
def current_timestamp_ns():
    return int(time.time() * 1e9)

# Générateur d'identifiant alphanumérique
def random_order_id(length=10):
    return ''.join(random.choices(string.ascii_uppercase + string.digits, k=length))

# Génération des données fictives
def generate_order():
    timestamp = current_timestamp_ns()
    order_id = random_order_id()
    instrument = random.choice(instruments)
    side = random.choice(sides)
    order_type = random.choice(types)
    quantity = random.randint(1, 1000)
    price = round(random.uniform(10, 1000), 2) if order_type == 'LIMIT' else None
    action = random.choice(actions)
    
    return {
        'timestamp': timestamp,
        'order_id': order_id,
        'instrument': instrument,
        'side': side,
        'type': order_type,
        'quantity': quantity,
        'price': price,
        'action': action
    }

# Nombre d'ordres à générer
num_orders = 100
orders = [generate_order() for _ in range(num_orders)]

# Création du DataFrame
df = pd.DataFrame(orders)

# Export CSV
df.to_csv(output_file, index=False)

print(f"Fichier {output_file} généré avec succès.")
