// sala - a component of the depthmapX - spatial network analysis platform

// THIS CODE IS DEPRECATED DO NOT USE: USE SHAPE MAPS INSTEAD 21.08.05

// INCLUDED ONLY TO ALLOW COMPATIBILITY WITH OLD VERSIONS

// DataLayer.cpp: implementation of the DataLayer class.
//
//////////////////////////////////////////////////////////////////////

#include <math.h>

#include <sala/mgraph.h>
#include <sala/datalayer.h>

bool DataLayers::read(ifstream& stream, int version)
{
   m_layers.clear();
   stream.read( (char *) &m_current_layer_index, sizeof(int) );
   int size;
   stream.read( (char *) &size, sizeof(int) );
   for (int i = 0; i < size; i++) {
      DataLayer data;
      m_layers.push_back(data);
      m_layers.tail().read(stream, version);
   }
   return true;
}

bool DataLayers::write(ofstream& stream)
{
   stream.write( (char *) &m_current_layer_index, sizeof(int) );
   int size = m_layers.size();
   stream.write( (char *) &size, sizeof(int) );
   for (size_t i = 0; i < m_layers.size(); i++) {
      m_layers[i].write(stream);
   }
   return true;
}

////////////////////////////////////////////////////////

void DataLayer::setDisplayColumn(int i)
{
   if (i == 0) {
      for (size_t j = 0; j < m_data_objects.size(); j++) {
         m_data_objects[j].m_color = float(j + 1) / m_data_objects.size();
      }
      m_display_min = 0;
      m_display_max = m_data_objects.size();
   }
   else if (m_data_objects.size()) {
      double max = m_data_objects[0][i-1], min = m_data_objects[0][i-1];
      for (size_t j = 1; j < m_data_objects.size(); j++) {
         if (m_data_objects[j][i-1] < min) {
            min = m_data_objects[j][i-1];
         }
         else if (m_data_objects[j][i-1] > max) {
            max = m_data_objects[j][i-1];
         }
      }
      for (size_t k = 0; k < m_data_objects.size(); k++) {
         m_data_objects[k].m_color = (float) ((m_data_objects[k][i-1] - min) / (max - min));
      }
      m_display_min = min;
      m_display_max = max;
   }
   m_display_column = i;
}

bool DataLayer::read(ifstream& stream, int version)
{
   m_data_objects.clear();
   m_column_titles.clear();
   stream.read( (char *) &m_layer_ref, sizeof(int) );
   m_layer_name.read(stream);
   stream.read( (char *) &m_next_object_ref, sizeof(int) );
   int rows_size;
   stream.read( (char *) &rows_size, sizeof(int) );
   for (int i = 0; i < rows_size; i++) {
      DataObject object;
      m_data_objects.push_back(object);
      m_data_objects.tail().read(stream,version);
   }
   int columns_size;
   stream.read( (char *) &columns_size, sizeof(int) );
   for (int j = 0; j < columns_size; j++) {
      pstring title;
      m_column_titles.push_back(title);
      m_column_titles.tail().read(stream);
   }

   // Since it doesn't seem to be recorded, just display the initial column
   setDisplayColumn(0);

   return true;
}

bool DataLayer::write(ofstream& stream)
{
   stream.write( (char *) &m_layer_ref, sizeof(int) );
   m_layer_name.write(stream);
   stream.write( (char *) &m_next_object_ref, sizeof(int) );
   int rows_size = m_data_objects.size();
   stream.write( (char *) &rows_size, sizeof(int) );
   for (size_t i = 0; i < m_data_objects.size(); i++) {
      m_data_objects[i].write(stream);
   }
   int columns_size = m_column_titles.size();
   stream.write( (char *) &columns_size, sizeof(int) );
   for (int j = 0; j < columns_size; j++) {
      m_column_titles[j].write(stream);
   }
   return true;
}

bool DataLayer::output(ofstream& stream)
{
   stream << "Ref" << "\t" << "x" << "\t" << "y" << "\t";
   for (size_t i = 0; i < m_column_titles.size(); i++) {
      stream << m_column_titles[i] << "\t";
   }
   stream << endl;

   for (size_t j = 0; j < m_data_objects.size(); j++) {
      stream.precision(12);
      stream << j << "\t" << m_data_objects[j].getCentroid().x << "\t" << m_data_objects[j].getCentroid().y << "\t";
      stream.precision(7);
      for (size_t k = 0; k < m_column_titles.size(); k++) {
         stream << m_data_objects[j].m_data_cols[k] << "\t";
      }
      stream << endl;
   }

   return true;
}

//////////////////////////////////////////////////////////////

bool DataObject::read(ifstream& stream, int version)
{
   stream.read( (char *) &m_object_ref, sizeof(int) );
   m_object_name.read(stream);
   m_data_cols.read(stream);
   if (version >= VERSION_LAYERS_CENTROID_INTROD) {
      stream.read( (char *) &m_centroid, sizeof(Point2f) );
   }
   // default values for selection / output color
   m_selected = false;
   m_color = 0.0f;

   return true;
}

bool DataObject::write(ofstream& stream)
{
   stream.write( (char *) &m_object_ref, sizeof(int) );
   m_object_name.write(stream);
   m_data_cols.write(stream);
   stream.write( (char *) &m_centroid, sizeof(Point2f) );
   return true;
}

