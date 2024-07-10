import React, { useState, useEffect } from 'react';
import { Box, Button, Typography, useTheme } from "@mui/material";
import { tokens } from "../../theme";
import Header from "../../components/Header";
import GeographyChart from "../../components/GeographyChart";
import StatBox from "../../components/StatBox";
import TimerIcon from '@mui/icons-material/Timer';
import FlashOnIcon from '@mui/icons-material/FlashOn';
import SpeedIcon from '@mui/icons-material/Speed';
import AirIcon from '@mui/icons-material/Air';

const Dashboard = () => {
  const theme = useTheme();
  const colors = tokens(theme.palette.mode);

  const [realizaData, setRealizaData] = useState(null);
  const [tick, setTick] = useState(0);
  const [resetTime, setResetTime] = useState(0);
  const [resetSpeed, setResetSpeed] = useState(0);

  useEffect(() => {
    const intervalId = setInterval(() => {
      setTick(tick => tick + 1);
    }, 100);
    return () => clearInterval(intervalId);
  }, []);

  const fetchData = async () => {
    try {
      const realizaResponse = await fetch('http://localhost:5000/data');
      if (!realizaResponse.ok) {
        throw new Error('Failed to fetch realiza data');
      }
      const realizaData = await realizaResponse.json();
      setRealizaData(realizaData);

      console.log('realizaData', realizaData);
    } catch (error) {
      console.error('Error fetching data', error);
    }
  };

  useEffect(() => {
    fetchData();
  }, [tick]);

  const handleReset = () => {
    if (realizaData) {
      setResetTime(realizaData.tempo);
      localStorage.setItem('resetTime', realizaData.tempo);
      setResetSpeed(realizaData.velocidadeInstantanea);
      
      localStorage.setItem('horaInicio', Date.now());

    }
  };

  return (
    <Box m="20px">
      {/* HEADER */}
      <Box display="flex" justifyContent="space-between" alignItems="center">
        <Header title="CARRINHO DE LINHA" subtitle="PI 1 | Turma 4 | Grupo 2 | Projeto de Algoritmo" />

        <Button
          onClick={handleReset}
          sx={{
            backgroundColor: colors.primary[400],
            color: colors.grey[100],
            fontSize: "14px",
            fontWeight: "bold",
            padding: "10px 20px",
          }}
        >
          Início
        </Button>
      </Box>

      {/* GRID & CHARTS */}
      <Box
        display="grid"
        gridTemplateColumns="repeat(12, 1fr)"
        gridAutoRows="140px"
        gap="20px"
      >
        {/* ROW 1 */}
        {realizaData && (
          <>
            {/* TEMPO */}
            <Box
              gridColumn="span 2"
              backgroundColor={colors.primary[400]}
              display="flex"
              alignItems="center"
              justifyContent="center"
            >
              <StatBox
                title={`${realizaData.tempo - resetTime} s`}
                subtitle="Tempo"
                progress="0.50"
                increase=""
                icon={
                  <TimerIcon
                    sx={{ color: colors.greenAccent[600], fontSize: "26px" }}
                  />
                }
              />
            </Box>

            {/* CONSUMO ENERGETICO */}
            <Box
              gridColumn="span 2"
              backgroundColor={colors.primary[400]}
              display="flex"
              alignItems="center"
              justifyContent="center"
            >
              <StatBox
                title={`${realizaData.consumoEnergetico} KWh`}
                subtitle="Consumo Energético"
                progress="0.80"
                increase=""
                icon={
                  <FlashOnIcon
                    sx={{ color: colors.greenAccent[600], fontSize: "26px" }}
                  />
                }
              />
            </Box>
          </>
        )}

        {/* TRAJETORIA */}
        {/* <Box
          gridColumn="span 4"
          gridRow="span 3"
          backgroundColor={colors.primary[400]}
          padding="30px"
        >
          <Typography
            variant="h5"
            fontWeight="600"
            sx={{ marginBottom: "15px" }}
          >
            Trajetória
          </Typography>
          <Box height="200px">
            <GeographyChart isDashboard={true} />
          </Box>
        </Box> */}

        {/* ROW 2 */}
        {/* VELOCIDADE */}
        <Box
          gridColumn="span 2"
          backgroundColor={colors.primary[400]}
          display="flex"
              alignItems="center"
              justifyContent="center"
            >
              <StatBox
                 title={realizaData ? `${(realizaData.velocidadeInstantanea - resetSpeed).toFixed(5)} s` : 'Carregando...'}
                subtitle="Velocidade Instantânea"
                progress="0.50"
                increase=""
                icon={
                  <SpeedIcon
                    sx={{ color: colors.greenAccent[600], fontSize: "26px" }}
                  />
                }
              />
            </Box>
        {/* ACELERAÇÃO */}
        <Box
          gridColumn="span 2"
          backgroundColor={colors.primary[400]}
          display="flex"
              alignItems="center"
              justifyContent="center"
            >
              <StatBox
                 title={realizaData ? `${realizaData.aceleracaoInstantanea.toFixed(5)} s` : 'Carregando...'}
                subtitle="Aceleração Instantânea"
                progress="0.50"
                increase=""
                icon={
                  <AirIcon
                    sx={{ color: colors.greenAccent[600], fontSize: "26px" }}
                  />
                }
              />
            </Box>
      </Box>
    </Box>
  );
};

export default Dashboard;
