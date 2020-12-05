/**************************************************************************************************************************/
/**
 * @brief PictureGraphics::buildMonoImage. Build mono image
 */
void PictureGraphics::buildMonoImage()
{
    datosFinal.clear();
    uint8_t pos= 8;
    int j=0;
    int contador=0;
//    int i=0;
    for (int i=0; i < this->actualHeight ; i++)
    {
        j=0;
        while((j < actualWidth) && (contador < datos_list.size()))
        {
            datosFinal.append(extractBit(datos_list.at(contador), (pos-=1))) ;
            if(pos == 0)
            {
                pos=8;
                contador++;
            }
            j++;
        }
        //i = i+j;
        if(pos > 0 && pos <8)
        {
            pos = 8;
            contador++;
        }
    }
}
/**************************************************************************************************************************/
/**
 * @brief PictureGraphics::build16colorImage. Build 16 color image
 */
void PictureGraphics::build16colorImage()
{
    datosFinal.clear();
    int contador=0;
    uint8_t uno;
    uint8_t dos;
    for (int i=0; i< this->actualHeight; i++)
    {
        for(int j=0; j < this->actualWidth; j+=2)
        {
            uno = (this->datos_list.at(contador) & 0xF0) >> 4;
            datosFinal.append(uno);

            if( ((j< actualWidth-1) )|| (j == actualWidth-1 && actualWidth%2==0))
            {
                dos = this->datos_list.at(contador) & 0x0F;
                datosFinal.append(dos);
            }
            contador++;
        }
    }
}
/**************************************************************************************************************************/
/**
 * @brief PictureGraphics::descomprimirDatos. Function to decompress encoded data
 * @param datosSinComprimir
 */
void PictureGraphics::decompressData()
{
    QList<uint8_t> datosAux;
    for(int i=0; i < datos_list.size(); i++)
    {
        for(int a=0; a < datos_list.at(i); a++) //First byte indicate the number of times to second byte appear
        {
            uint8_t dato = datos_list.at(i+1);
            this->datosFinal.append(dato);
            datosAux.append(dato);
        }
        i+=1;
    }
    datos_list.clear();
    datos_list = datosAux;
}
