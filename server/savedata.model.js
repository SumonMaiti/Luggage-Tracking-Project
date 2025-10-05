import mongoose, { Schema } from "mongoose";

const saveDataSchema = new Schema(
  {
    deviceId: {
      type: String,
      required: true,
      index: true,
      unique: true,
    },
    data: {
      type: Map,
      of: String, // All values are stored as strings
      default: {},
    },
  },
  { timestamps: true }
);

export const saveData =
  mongoose.models.saveDatas || mongoose.model("saveDatas", saveDataSchema);
