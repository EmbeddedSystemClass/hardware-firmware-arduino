#include <Fat16.h>

class XReaderBase {
  public:
    virtual uint32_t curPosition(void);
    virtual uint8_t seekSet(uint32_t pos);
    virtual int16_t read(void);
    virtual uint32_t fileSize(void);
};

class XFileReader : public XReaderBase { 
  public:
    Fat16* m_pDataFile;
  
  public:
    void SetFile(Fat16* pDataFile) { m_pDataFile = pDataFile; }
    virtual uint32_t curPosition(void) { return m_pDataFile->curPosition(); }
    virtual uint8_t seekSet(uint32_t pos) { return m_pDataFile->seekSet(pos); }
    virtual int16_t read(void) { return m_pDataFile->read(); }
    virtual uint32_t fileSize(void) {return m_pDataFile->fileSize();}
};

class XByteArrayReader : public XReaderBase {
  public:
    uint8_t* m_pArray;
    uint8_t* m_pCurrent;
    uint32_t m_Count;
      
  public:
    void SetArray(uint8_t* pArray, uint32_t count)
    { 
      m_pArray = pArray;
      m_pCurrent = pArray;
      m_Count = count;      
    }
    
    virtual uint32_t curPosition(void) 
    { 
      return m_pCurrent - m_pArray; 
    }
    
    virtual uint8_t seekSet(uint32_t pos)
    { 
      if(pos < m_Count) {
        m_pCurrent = m_pArray + pos;
        return true; 
      }
      m_pCurrent = m_pArray;
      return false;
    }
    
    virtual int16_t read(void)
    { 
      return *m_pCurrent++;       
    }
    
    virtual uint32_t fileSize(void) 
    {
      return m_Count;
    }
};
