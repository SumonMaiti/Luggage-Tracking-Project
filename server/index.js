import dotenv from 'dotenv';
import express from 'express';
import { saveData } from './savedata.model.js';
import { ConnectMongoDB } from './MongoDBConnection.js';

dotenv.config();
const app = express();

app.get('/', (req, res) => {
  res.send('Hello World!');
});

// ✅ Save or update fieldName + fieldVal by device ID
app.get('/save', async (req, res) => {
  const { id, fieldName, fieldVal } = req.query;

  if (!id || !fieldName || !fieldVal) {
    return res.status(400).send('Missing id, fieldName, or fieldVal');
  }

  try {
    await ConnectMongoDB();

    const update = { [`data.${fieldName}`]: fieldVal };
    const options = { upsert: true, new: true, setDefaultsOnInsert: true };

    const result = await saveData.findOneAndUpdate(
      { deviceId: id },
      { $set: update, $setOnInsert: { deviceId: id } },
      options
    );

    console.log(`✅ Updated [${id}]: { ${fieldName}: ${fieldVal} }`);
    res.send(`✅ Saved data for device ${id}`);
  } catch (err) {
    console.error('❌ Save error:', err);
    res.status(500).send('Failed to save data.');
  }
});

// ✅ Fetch all saved data by device ID
app.get('/fetch', async (req, res) => {
  const { id } = req.query;

  if (!id) {
    return res.status(400).send('Missing id');
  }

  try {
    await ConnectMongoDB();

    const result = await saveData.findOne({ deviceId: id });

    if (!result) {
      return res.status(404).send('No data found for this ID');
    }

    res.json({
      deviceId: result.deviceId,
      data: Object.fromEntries(result.data), // Convert Map to object
      createdAt: result.createdAt,
      updatedAt: result.updatedAt
    });
  } catch (err) {
    console.error('❌ Fetch error:', err);
    res.status(500).send('Failed to fetch data.');
  }
});

// ✅ Start server
const port = process.env.PORT || 3000;
app.listen(port, () => {
  console.log(`🚀 Server running on http://localhost:${port}`);
});
