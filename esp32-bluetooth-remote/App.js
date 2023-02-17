import { StatusBar } from 'expo-status-bar';
import { useState } from 'react';
import { StyleSheet, Text, View, Button } from 'react-native';

const buttonCodes = {
  'power': 1,
  'mute': 2,
  'decrease': 3,
  'increase': 4,
  'line1': 5,
  'line2': 6,
  'opt': 7,
  'cox': 8,
  'bt': 9,
  'play': 10,
  'prev': 11,
  'next': 12
};

export default function App() {
  const [pressedButton, setPressedButton] = useState(0);

  function handleClick(id) {
    setPressedButton(buttonCodes[id]);
    return buttonCodes[id];
  }

  return (
    <View style={styles.container}>
      <Text>{pressedButton}</Text>
      <Button title='Power' onPress={() => handleClick('power')}></Button>
      <Button title='Mute' onPress={() => handleClick('mute')}></Button>
      <Button title='Decrease' onPress={() => handleClick('decrease')}></Button>
      <Button title='Increase' onPress={() => handleClick('increase')}></Button>
      <Button title='Line1' onPress={() => handleClick('line1')}></Button>
      <Button title='Line2' onPress={() => handleClick('line2')}></Button>
      <Button title='OPT' onPress={() => handleClick('opt')}></Button>
      <Button title='COX' onPress={() => handleClick('cox')}></Button>
      <Button title='Bluetooth' onPress={() => handleClick('bt')}></Button>
      <Button title='Play' onPress={() => handleClick('play')}></Button>
      <Button title='Previous' onPress={() => handleClick('prev')}></Button>
      <Button title='Next' onPress={() => handleClick('next')}></Button>
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: '#fff',
    alignItems: 'center',
    justifyContent: 'center',
  },
});
