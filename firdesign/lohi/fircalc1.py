#!/usr/bin/env python
import sys
from PyQt5 import  QtCore, QtGui, QtWidgets, uic

import numpy as np
import scipy.signal as signal
import matplotlib.pyplot as plt
import math
import os

qtCreatorFile = "fircalc1.ui"

ui_path = os.path.dirname(os.path.abspath(__file__))
Ui_MainWindow, QtBaseClass  = uic.loadUiType(os.path.join(ui_path, qtCreatorFile))

class MyApp(QtWidgets.QMainWindow, Ui_MainWindow):
    def __init__(self):
        QtWidgets.QMainWindow.__init__(self)
        Ui_MainWindow.__init__(self)
        self.setupUi(self)
        self.textEditStopBand.setText("-60")
        self.textEditPassRipple.setText("0.01")
        self.TextCutoff.setText("50.0")
        self.TextSampleRate.setText("1000")
        self.TextTransitionFreq.setText("20")
        self.FirWindowHamming.setChecked(True)
        self.radioBLP.setChecked(True)
        self.CalcButton.clicked.connect(self.CalculateFir)
    def CalculateTaps(self, Fs, Tf ): #Bellanger's classic Digital Processing of Signals – Theory and Practice
        stop_band =  float(self.textEditStopBand.toPlainText())
        stop_band_value = 10**( stop_band/20) 
        passband_ripple = float(self.textEditPassRipple.toPlainText())/100
        temp = 1.0/( 10* passband_ripple * stop_band_value )
        Ntaps = (int) ( (2.0/3.0) * ( math.log10( temp) * ( Fs/ Tf) ) )
        print("StopBand = ", stop_band )
        print("StopBandValue = ", stop_band_value )
        print("passband_ripple = ", passband_ripple)
        print("Num Taps = ", Ntaps)
        return(Ntaps)
    def CalculateFir(self):
        sample_rate = float(self.TextSampleRate.toPlainText())
        cutoff_freq =  float(self.TextCutoff.toPlainText())
        transition_freq = int(self.TextTransitionFreq.toPlainText())

        if( cutoff_freq >  sample_rate/2 ):
          self.textEditFileName.setText("Error: Cutoff above Nyquist")
          return  

        num_taps = self.CalculateTaps( sample_rate, transition_freq )
        if( num_taps % 2 ) == 0:
          num_taps = num_taps +1
          
        strwindow = 'hamming'
        if( self.FirWindowHamming.isChecked() == True ):
          strwindow = 'hamming'
        elif( self.FirwindowBlackman.isChecked() == True ):  
          strwindow = 'blackman'
        else:
          strwindow = 'hann'

        strfilter = 'lowpass'
        strfilename = ui_path + '/lpfircoeff.txt'
        if( self.radioBLP.isChecked() == True ):       
          strfilter = 'lowpass'
          strfilename = ui_path + '/lpfircoeff.txt'
        elif( self.radioBHP.isChecked() == True ): 
          strfilter = 'highpass'
          strfilename = ui_path + '/hpfircoeff.txt'
       
                 
        b1 = signal.firwin( num_taps, cutoff_freq, None, strwindow, strfilter, True, sample_rate/2)

        # copy to file   
        f= open(strfilename, 'w') 
        strfilterspec = 'sample rate= ' + str(sample_rate) + 'Hz  cutofffreq= ' + str(cutoff_freq) + 'Hz  transition_freq=' +  str(transition_freq)
        strfilterspec = strfilterspec +  'Hz  window= ' + strwindow + '   Filter= ' + strfilter
        f.write(strfilterspec)
        f.write('\n')
        f.writelines( '\n'.join(map(str, b1)) )
        f.close()


        self.textEditFileName.setText("Saved to file " + strfilename)
        w1, h1 = signal.freqz(b1)
        w2 = w1 * (sample_rate/(2*math.pi) )
        plt.title('Digital filter frequency response')
        textstr = "Number Taps = " + str(num_taps) 

        plt.figtext(0.02, .95, textstr, fontsize=10)
        plt.plot(w2, 20*np.log10(np.abs(h1)), 'b')
        plt.ylabel('Amplitude Response (dB)')
        plt.xlabel('Frequency Hz')
        plt.grid()
        plt.show()
     
if __name__ == "__main__":
    app =  QtWidgets.QApplication(sys.argv)
    window = MyApp()
    window.show()
    sys.exit(app.exec_())
