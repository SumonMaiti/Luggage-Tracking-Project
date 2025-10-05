import mongoose from 'mongoose';
import dotenv from 'dotenv';

// Load environment variables
dotenv.config();


const ConnectMongoDB = async () => {

  const uri = process.env.MONGO_URI;

  const dbName = process.env.DBName
  await mongoose.connect(uri, {
    dbName: dbName
  });


}
const DisConnectMongoDB = async () => {

  await mongoose.disconnect();


}


export { ConnectMongoDB, DisConnectMongoDB };