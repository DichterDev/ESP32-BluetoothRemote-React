import axios from 'axios';
import { StatusBar } from 'expo-status-bar';
import { useEffect, useRef, useState } from 'react';
import { StyleSheet, Text, View, TouchableOpacity } from 'react-native';
import Feather from '@expo/vector-icons/Feather';

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

  const [iconColor, setIconColor] = useState('#E88873');
  let intervalCheck = useRef(true);
  let intervalCheckPrev = useRef(true);

  function resetIconColor() {
    setIconColor('#E88873');
  }

  async function handleClick(id) {
    if(!intervalCheck.current && !intervalCheckPrev.current) return;
    
    intervalCheck.current = false;
    setIconColor('red');
    let code = buttonCodes[id];
    let res = await axios.post('http://192.168.0.199/api', {
      cmd: code
    }, { headers: {
      'Content-Type': 'application/json'
    }});
  }

  useEffect(() => {
    const interval = setInterval(() => {
      if(intervalCheck.current && intervalCheckPrev.current) {
        resetIconColor();
      }
      intervalCheckPrev.current = intervalCheck.current;
      intervalCheck.current = true;
    }, 300);

    return () => clearInterval(interval);
  }, [])

  return (
    <View style={{height: '100%', width: '100%', backgroundColor: '#F3E9D2'}}>
      <View style={styles.wrapper}>
        <View style={styles.container}>
          <TouchableOpacity style={styles.button} onPress={() => handleClick('power')}>
            <Feather style={[styles.icon, {color: iconColor}]} name='power'></Feather>
          </TouchableOpacity>
        </View>
        <View style={styles.container}>
          <TouchableOpacity style={styles.button} onPress={() => handleClick('mute')}>
            <Feather style={[styles.icon, {color: iconColor}]} name='volume-x'></Feather>
          </TouchableOpacity>
        </View>
        <View style={styles.container}>
          <TouchableOpacity style={styles.button} onPress={() => handleClick('decrease')}>
            <Feather style={[styles.icon, {color: iconColor}]} name='volume-1'></Feather>
          </TouchableOpacity>
        </View>
        <View style={styles.container}>
          <TouchableOpacity style={styles.button} onPress={() => handleClick('increase')}>
            <Feather style={[styles.icon, {color: iconColor}]} name='volume-2'></Feather>
          </TouchableOpacity>
        </View>
        <View style={styles.container}>
          <TouchableOpacity style={styles.button} onPress={() => handleClick('line1')}>
            <Text style={[styles.text, {color: iconColor}]}>Line1</Text>
          </TouchableOpacity>
        </View>
        <View style={styles.container}>
          <TouchableOpacity style={styles.button} onPress={() => handleClick('line2')}>
            <Text style={[styles.text, {color: iconColor}]}>Line2</Text>
          </TouchableOpacity>
        </View>
        <View style={styles.container}>
          <TouchableOpacity style={styles.button} onPress={() => handleClick('opt')}>
            <Text style={[styles.text, {color: iconColor}]}>OPT</Text>
          </TouchableOpacity>
        </View>
        <View style={styles.container}>
          <TouchableOpacity style={styles.button} onPress={() => handleClick('cox')}>
            <Text style={[styles.text, {color: iconColor}]}>COX</Text>
          </TouchableOpacity>
        </View>
        <View style={styles.container}>
          <TouchableOpacity style={styles.button} onPress={() => handleClick('bt')}>
            <Feather style={[styles.icon, {color: iconColor}]} name='bluetooth'></Feather>
          </TouchableOpacity>
        </View>
        <View style={styles.container}>
          <TouchableOpacity style={styles.button} onPress={() => handleClick('play')}>
            <Feather style={[styles.icon, {color: iconColor}]} name='play'></Feather>
          </TouchableOpacity>
        </View>
        <View style={styles.container}>
          <TouchableOpacity style={styles.button} onPress={() => handleClick('prev')}>
            <Feather style={[styles.icon, {color: iconColor}]} name='skip-back'></Feather>
          </TouchableOpacity>
        </View>
        <View style={styles.container}>
          <TouchableOpacity style={styles.button} onPress={() => handleClick('next')}>
            <Feather style={[styles.icon, {color: iconColor}]} name='skip-forward'></Feather>
          </TouchableOpacity>
        </View>
      </View>
    </View>
  );
}

const styles = StyleSheet.create({
  wrapper: {
    flex: 1,
    flexDirection: 'row',
    flexWrap: 'wrap',
    alignItems: 'flex-start',
    margin: 30
  },
  container: {
    height: '16.5%',
    width: '50%',
    alignItems: 'center',
    padding: 5,
  },
  button: {
    height: '100%',
    aspectRatio: 1,

    backgroundColor: '#484A47',
    borderRadius: 999,

    alignItems: 'center',
    justifyContent: 'center',
  },
  icon: {
    fontSize: 75
  },
  text: {
    fontSize: 35,
    fontWeight: '600'
  }
});